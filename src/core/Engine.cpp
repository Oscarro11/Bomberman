#include "core/Engine.hpp"
#include "ecs/PlayerStats.hpp"
#include "entities/Enemigo.hpp"
#include "entities/PowerUp.hpp"
#include "utils/AssetsUtils.hpp"
#include "systems/ScoreManager.hpp"

Engine::Engine(std::string tableroSource, std::vector<PlayerStats>& jugadores)
    : state_(MatchState::Preparing)
    , inputHandler_(this)
    , tablero_(tableroSource)
    , roundTimer_(sf::seconds(180.f))
    , enemiesSystem_(enemigos_, explosiones_, tablero_, eventBus_)
    , bombsSystem_(jugadores_, bombas_, explosiones_, enemigos_, tablero_, eventBus_)
{
    pthread_mutex_init(&inputMutex_, NULL);

    singlePlayer_ = (jugadores.size() == 1);

    const auto& playersSpawn = tablero_.getSpawnPlayers();
    
    for (int i=0; i < jugadores.size(); i++)
    {
        const PlayerStats& info = jugadores.at(i);

        Player player = Player(i, info.nombre, 3, info.maxBombas, info.rangoExplosion, playersSpawn[i].position.x, playersSpawn[i].position.y);
        this -> jugadores_.push_back(player);
        
        switch (i)
        {
        case 0: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player1, i}); break;
        case 1: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player2, i}); break;
        case 2: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player3, i}); break;
        case 3: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player4, i}); break;
        default: break;}

    };
    const auto& enemySpawns = tablero_.getSpawnEnemies();

    int enemyId = 0;
    for (const Position& pos : enemySpawns)
    {
        enemigos_.push_back(Enemigo(enemyId, 1, 3, pos.x, pos.y));

        tablero_.addOccupant(
            pos,
            Occupant{
                EntityType::Enemy,
                enemyId
            }
        );

        enemyId++;
    }
}

Engine::~Engine()
{
    bool started = (state_ != MatchState::Preparing);

    state_ = MatchState::Finished;
    eventBus_.stop();

    if (started){
        pthread_join(logicThread_, NULL);
        pthread_join(enemyThread_, NULL);
    }
    
    pthread_mutex_destroy(&inputMutex_);
}

void Engine::start()
{
    if (state_ != MatchState::Preparing) return;

    state_ = MatchState::Playing;

    // Logic thread
    pthread_create(&logicThread_, nullptr, logic_thread, this);

    // Enemies thread
    pthread_create(&enemyThread_, nullptr, enemy_thread, this);
}

void Engine::update(sf::Time dt)
{
    if (state_ != MatchState::Playing) return;

    roundTimer_ -= dt;
    bombsSystem_.update(dt);

    updatePlayersState();
    updateGameState();
}

// logic_thread
void* Engine::logic_thread(void* arg)
{
    Engine* engine = static_cast<Engine*>(arg);

    while (engine -> state() == MatchState::Playing)
    {
        auto evento = engine -> popEvento();

        if (evento) engine -> procesarEvento(*evento);
    }

    return nullptr;
}

void* Engine::enemy_thread(void* arg)
{
    Engine* engine = static_cast<Engine*>(arg);

    sf::Clock clock;

    while (engine -> state() == MatchState::Playing)
    {
        sf::Time dt = clock.restart();
        engine->enemiesSystem_.update(dt);
        sf::sleep(sf::milliseconds(16));
    }

    return nullptr;
}

void Engine::pushEvento(const Evento &evento)
{
    eventBus_.push(evento);
}

std::optional<Evento> Engine::popEvento()
{
    return eventBus_.pop();
}

void Engine::procesarEvento(Evento& evento){
    switch (evento.tipo())
    {
        case EventType::PlayerMove:
            onPlayerMove(evento);
            break;
            
        case EventType::EnemyMove:
            {
                auto damaged = enemiesSystem_.processMove(evento);

                if (damaged) danioPlayer(*damaged);
                break;
            } 
        
        case EventType::PlayerPlaceBomb:
            onPlayerPlaceBomb(evento);
            break;

        case EventType::BombExplode:
            onBombExplode(evento);
            break;

        case EventType::PlayerDamage:
            danioPlayer(evento.objetivo());
            break;

        case EventType::PlayerDeath:
            //onPlayerDeath(evento);
            break;

        case EventType::TileDestroyed:
            onTileDestroyed(evento);
            break;
        
        case EventType::EnemyDeath:
            onEnemyDeath(evento);
            break;

        case EventType::ChainExplosion:
            onChainExplosion(evento);
            break;

        case EventType::PlayerPickPowerUp:
            onPlayerPickPowerUp(evento);
            break;
    }
}

void Engine::updatePlayersState()
{
    for (Player& player : jugadores_)
    {
        player.actualizarInvencibilidad();
    }
}

void Engine::updateGameState()
{
    int alivePlayers = 0;

    for (const Player& p : jugadores_)
    {
        if (p.vida() > 0)
            ++alivePlayers;
    }

    bool noEnemies = !enemiesSystem_.hasLivingEnemies();

    if (singlePlayer_)
    {
        if (alivePlayers == 0) state_ = MatchState::WaitingForGameOverConfirmation;
        if (noEnemies) state_ = MatchState::WaitingForGameOverConfirmation;
    }
    else
    {
        if (alivePlayers <= 1) state_ = MatchState::WaitingForGameOverConfirmation;
    }
    if (roundTimer_ <= sf::Time::Zero) state_ = MatchState::WaitingForGameOverConfirmation;
}

RenderSnapshot Engine::makeRenderSnapshot()
{
    RenderSnapshot snapshot;

    int width  = tablero_.getWidth();
    int height = tablero_.getHeight();

    snapshot.tiles.resize(height, std::vector<TileType>(width));

    snapshot.entities.resize(height, std::vector<EntityType>(width, EntityType::None));

    // Copy board state
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Position pos{x, y};

            const BoardCell& cell = tablero_.getCell(pos);
            snapshot.tiles[y][x] = cell.terrainType;

            // Simplified:
            // render first occupant only
            if (!cell.occupants.empty())
            {
                for (Occupant occ : cell.occupants) {
                    if (occ.type == EntityType::Explosion)
                    {
                        snapshot.entities[y][x] = EntityType::Explosion;
                        break;
                    }

                    if (occ.type == EntityType::Enemy)
                    {
                        snapshot.entities[y][x] = EntityType::Enemy;
                        break;
                    } 
                }
                
                if (snapshot.entities[y][x] == EntityType::None)
                {
                    snapshot.entities[y][x] = cell.occupants.front().type;    
                }
            }
        }
    }

    // Copy player data
    for (const Player& p : jugadores_)
    {
        snapshot.players.push_back(PlayerData{
            p.nombre(),
            p.id(),
            p.vida(),
            p.maxBombas(),
            p.restBombas(),
            p.rangoExplosion(),
            p.puntaje()
        });
    }

    // HUD data
    snapshot.hud.roundTime = roundTimer_.asSeconds();
    snapshot.hud.gameOver  = (state_ == MatchState::WaitingForGameOverConfirmation);
    snapshot.hud.numPlayers = jugadores_.size();

    return snapshot;
}

void Engine::confirmGameOver() {
    if (state_ != MatchState::WaitingForGameOverConfirmation)
        return;

    // Award survival bonus to winner
    int winnerId = getWinnerId();
    
    if (winnerId >= 0)
        jugadores_[winnerId].anadirPuntos(300 + (int)roundTimer_.asSeconds() * 10);

    // Save every player's result
    for (const Player& p : jugadores_) {
        if (p.nombre() != ""){
                ScoreManager::save(ScoreEntry{
                p.nombre(),
                p.puntaje(),
                p.vida() > 0 ? 1 : 0,   // win: 1 if alive, 0 if not
                p.muertes()               // add deaths counter to Player
            });
        }
    }

    state_ = MatchState::Finished;
}

std::string Engine::getWinnerName() const {
    for (const Player& p : jugadores_)
        if (p.vida() > 0) return p.nombre();
    return "Nadie";   // round timer ran out
}

int Engine::getWinnerId() const {
    for (const Player& p : jugadores_)
        if (p.vida() > 0) return p.id();
    return -1;
}

void Engine::onPlayerMove(Evento &evento)
{
    int playerId = evento.autor();

    if (playerId < 0 || playerId >= jugadores_.size())
        return;

    Player& player = jugadores_[playerId];

    int dx = evento.data().mover.dx;
    int dy = evento.data().mover.dy;

    Position oldPos = player.getPosition();

    Position newPos{
        oldPos.x + dx,
        oldPos.y + dy
    };

    const BoardCell cell = tablero_.getCell(newPos);

    for (const Occupant& occ : cell.occupants)
    {
        if (occ.type == EntityType::Enemy)
        {
            danioPlayer(player.id());
            return;
        }

    }

    // Verificar si se puede caminar
    if (!tablero_.isWalkable(newPos))
    {
        return;
    }

    // Mover occupant en tablero
    bool moved = tablero_.moveOccupant(
        oldPos,
        newPos,
        player.id()
    );

    if (!moved)
    {
        return;
    }

    // Actualizar posicion interna del jugador
    player.setPosition(newPos.x, newPos.y);

    for (const Occupant& occ : cell.occupants)
    {
        if (occ.type == EntityType::Explosion)
        {
            danioPlayer(
                player.id()
            );

            break;
        }

        if (occ.type == EntityType::PowerUpItem)
        {
            pushEvento(Evento::playerPickPowerUp(player.id(), occ.entityId, newPos.x, newPos.y, powerUps_[occ.entityId].tipo()));
        }
    }
}

void Engine::danioPlayer(int playerId)
{
    if (playerId < 0 || playerId >= jugadores_.size())
        return;

    Player& player = jugadores_[playerId];

    if (player.esInvencible())
        return;

    player.anadirMuerte();
    player.activarInvencibilidad();

    auto deathEvent = player.recibirDanio(player);
    // Si el jugador muere, se procesa un evento de muerte. De lo contrario, se respawnea en su posicion inicial
    if (deathEvent.has_value())
    {
        pushEvento(deathEvent.value());
    }
    // Si el jugador no muere, se mueve a su posicion de spawn
    else
    {
        Position oldPos = player.getPosition();

        Position spawnPos{
            player.spawnPointX(),
            player.spawnPointY()
        };
        
        tablero_.moveOccupant(
            oldPos,
            spawnPos,
            player.id()
        );

        player.setPosition(
            spawnPos.x,
            spawnPos.y
        );
    }
    
}

void Engine::onPlayerPlaceBomb(Evento& evento)
{
    int playerId = evento.autor();

    if (playerId < 0 ||
        playerId >= static_cast<int>(jugadores_.size()))
    {
        return;
    }

    Player& player = jugadores_[playerId];

    bombsSystem_.placeBomb(
        playerId,
        player.posX(),
        player.posY(),
        player.rangoExplosion()
    );
}

void Engine::onBombExplode(Evento& evento)
{
    int creador = evento.data().explosion.creador;

    jugadores_[creador].recuperarBomba();

    bombsSystem_.explodeBomb(evento);
}

void Engine::onTileDestroyed(Evento& evento)
{
    Position pos{
        evento.posicionX(),
        evento.posicionY()
    };

    tablero_.setTerrain(
        pos,
        TileType::Floor
    );

    if (rand() % 100 < 20)
    {
        PowerUpType tipo =
            static_cast<PowerUpType>(
                rand() % 3
            );
        
        powerUps_.push_back(
            PowerUp(
                tipo,
                pos.x,
                pos.y
            )
        );

        tablero_.addOccupant(
            pos,
            Occupant{
                EntityType::PowerUpItem,
                static_cast<int>(powerUps_.size() - 1)
            }
        );
    }
}

void Engine::onEnemyDeath(Evento& evento)
{
    int enemyId = evento.objetivo();
    int playerId = evento.autor();

    enemiesSystem_.killEnemy(enemyId);
    jugadores_[playerId].anadirPuntos(100);

    Position pos{
        evento.posicionX(),
        evento.posicionY()
    };

    tablero_.removeOccupant(
        pos,
        EntityType::Enemy,
        enemyId
    );
}

void Engine::onChainExplosion(Evento& evento)
{
    Position pos{
        evento.posicionX(),
        evento.posicionY()
    };
    for (auto it = bombas_.begin();
     it != bombas_.end();
     ++it)
    {
        if (it->getId() == evento.autor())
        {
            pushEvento(
                Evento::bombExplode(
                    it->getId(),
                    it->getCreador(),
                    it->getX(),
                    it->getY(),
                    it->getRadio()
                )
            );

            bombas_.erase(it);

            break;
        }
    }
}

void Engine::onPlayerPickPowerUp(
    Evento& evento
)
{
    int playerId = evento.autor();
    int powerUpId = evento.objetivo();

    PowerUpType tipo =
        static_cast<PowerUpType>(
            evento.data().powerUp.tipo
        );

    jugadores_[playerId]
        .actualizarStat(
            tipo,
            1
        );

    tablero_.removeOccupant(
        Position{
            evento.posicionX(),
            evento.posicionY()
        },
        EntityType::PowerUpItem,
        powerUpId
    );
}

void Engine::handleInput(sf::Keyboard::Key key, int playerId)
{
    if (state_ == MatchState::WaitingForGameOverConfirmation){
        if (key == sf::Keyboard::Enter)
        {
            confirmGameOver();
        }
        
        return;
    }
    else if (state_ == MatchState::Playing)
    {
        pthread_mutex_lock(&inputMutex_);
        Player& p = jugadores_[playerId];
        pthread_mutex_unlock(&inputMutex_);

        const KeyMap& km = InputHandler::KEYMAPS[playerId];
        if      (key == km.up)    pushEvento(p.generarEventoMov(Directions::UP));
        else if (key == km.down)  pushEvento(p.generarEventoMov(Directions::DOWN));
        else if (key == km.left)  pushEvento(p.generarEventoMov(Directions::LEFT));
        else if (key == km.right) pushEvento(p.generarEventoMov(Directions::RIGHT));
        else if (key == km.bomb){
            std::optional<Evento> evento = p.colocarBomba();
            
            if (evento.has_value())
                pushEvento(evento.value());
        }
    }
}
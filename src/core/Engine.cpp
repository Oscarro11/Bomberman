#include "core/Engine.hpp"
#include "ecs/PlayerStats.hpp"
#include "entities/Enemigo.hpp"
#include "utils/AssetsUtils.hpp"

Engine::Engine(std::string tableroSource, std::vector<PlayerStats>& jugadores)
    : state_(MatchState::Preparing)
    , inputHandler_(this)
    , tablero_(tableroSource)
    , roundTimer_(sf::seconds(15.f))
    , enemiesSystem_(enemigos_, eventBus_, tablero_)
{
    pthread_mutex_init(&inputMutex_, NULL);
    pthread_mutex_init(&enemiesMutex_, NULL);

    const auto& playersSpawn = tablero_.getSpawnPlayers();
    
    for (int i=0; i < jugadores.size(); i++)
    {
        const PlayerStats& info = jugadores.at(i);

        Player player = Player(i, info.nombre, 3, info.maxBombas, info.rangoExplosion,  info.velocidad, playersSpawn[i].position.x, playersSpawn[i].position.y);
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
    eventBus_.stop();

    if (state_ != MatchState::Preparing){
        for (const pthread_t& thread : threads)
        {
            pthread_join(thread, NULL);
        }

        pthread_join(logicThread_, NULL);
    }

    state_ = MatchState::Finished;
    pthread_mutex_destroy(&inputMutex_);
    pthread_mutex_destroy(&enemiesMutex_);
}

void Engine::start()
{
    if (state_ != MatchState::Preparing) return;

    state_ = MatchState::Playing;
    // Player threads
    for (int i = 0; i < jugadores_.size(); ++i)
    {
        pthread_t thread;

        PlayerThreadArg* arg = new PlayerThreadArg{this, i};

        pthread_create(&thread, nullptr, player_thread_process, arg);

        threads.push_back(thread);
    }

    // Logic thread
    pthread_create(&logicThread_, nullptr, logic_thread, this);
}

void Engine::update(sf::Time dt)
{
    if (state_ != MatchState::Playing) return;

    roundTimer_ -= dt;

    enemiesSystem_.update(dt);
    updatePlayersState();
    updateGameState();
}

void* Engine::player_thread_process(void* arg)
{
    PlayerThreadArg* data = (PlayerThreadArg*) arg;
    Engine* engine = data -> engine;
    int playerId = data -> playerId;
    delete data;

    while (!engine -> isFinished()) {
        //Pendiente
        sf::sleep(sf::milliseconds(16));
    }

    return nullptr;
}

// logic_thread
void* Engine::logic_thread(void* arg)
{
    Engine* engine = static_cast<Engine*>(arg);

    while (!engine -> isFinished())
    {
        auto evento = engine -> popEvento();

        if (evento) engine -> procesarEvento(*evento);
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
            
            auto damaged = enemiesSystem_.processMove(evento);

            if (damaged) danioPlayer(*damaged);
            break;
            
            /*
            case EventType::PlayerDeath:
                onPlayerDeath(evento);
                break;

        case EventType::PlayerPlaceBomb:
            onPlayerPlaceBomb(evento);
            break;


        case EventType::PlayerPickPowerUp:
            onPlayerPickPowerUp(evento);
            break;

        case EventType::BombExplode:
            onBombExplode(evento);
            break;

        case EventType::ChainExplosion:
            onChainExplosion(evento);
            break;


        case EventType::EnemyDeath:
            onEnemyDeath(evento);
            break;

        case EventType::TileDestroyed:
            onTileDestroyed(evento);
            break;

        case EventType::GameOver:
            onGameOver(evento);
            break;

        case EventType::RoundStart:
            onRoundStart(evento);
            break;
            */
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
        if (p.vida() > 0) ++alivePlayers;
    }

    if (alivePlayers <= 1) state_ = MatchState::WaitingForGameOverConfirmation;
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
                snapshot.entities[y][x] = cell.occupants.front().type;
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
            static_cast<int>(p.velocidad())
        });
    }

    // HUD data
    snapshot.hud.roundTime = roundTimer_.asSeconds();
    snapshot.hud.gameOver  = (state_ == MatchState::WaitingForGameOverConfirmation);
    snapshot.hud.numPlayers = jugadores_.size();

    return snapshot;
}

void Engine::confirmGameOver() {
    if (state_ ==
        MatchState::WaitingForGameOverConfirmation)
    {
        state_ = MatchState::Finished;
    }
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

    // Limites del tablero
    if (newPos.x < 0 ||
        newPos.y < 0 ||
        newPos.y >= tablero_.matrix().size() ||
        newPos.x >= tablero_.matrix()[0].size())
    {
        return;
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
}

void Engine::danioPlayer(int playerId)
{
    if (playerId < 0 || playerId >= jugadores_.size())
        return;

    Player& player = jugadores_[playerId];

    if (player.esInvencible())
        return;

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
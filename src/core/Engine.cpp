#include "core/Engine.hpp"
#include "ecs/PlayerStats.hpp"
#include "entities/Enemigo.hpp"
#include "utils/AssetsUtils.hpp"


Engine::Engine(std::string tableroSource, std::vector<PlayerStats>& jugadores)
    : running_(false)
    , inputHandler_(this)
    , tablero_(tableroSource)
    , roundTimer_(sf::seconds(180.f))
    , gameOver_(false)
{
    pthread_mutex_init(&eventMutex_, NULL);
    pthread_mutex_init(&inputMutex_, NULL);
    pthread_mutex_init(&enemiesMutex_, NULL);
    pthread_cond_init(&eventReady_, NULL);

    const auto& playersSpawn = tablero_.getSpawnPlayers();
    
    for (int i=0; i < jugadores.size(); i++)
    {
        const PlayerStats& info = jugadores.at(i);

        Player player = Player(i, 3, info.maxBombas, info.rangoExplosion,  info.velocidad, playersSpawn[i].position.x, playersSpawn[i].position.y);
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
    if (running_.load())
    {
        pthread_mutex_lock(&eventMutex_);
        running_.store(false);
        pthread_cond_broadcast(&eventReady_);
        pthread_mutex_unlock(&eventMutex_);

        for (const pthread_t& thread : threads)
        {
            pthread_join(thread, NULL);
        }

        pthread_join(logicThread_, NULL);
    }

    pthread_mutex_destroy(&eventMutex_);
    pthread_mutex_destroy(&inputMutex_);
    pthread_mutex_destroy(&enemiesMutex_);
    pthread_cond_destroy(&eventReady_);
}

void Engine::start()
{
    if (running_.load()) return;

    running_.store(true);

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
    if (gameOver_) return;

    roundTimer_ -= dt;

    enemyMoveTimer_ += dt;
    if (enemyMoveTimer_.asSeconds() >= ENEMY_MOVE_INTERVAL) {
        pthread_mutex_lock(&enemiesMutex_);
        moveEnemies();   // reads enemigos_ positions safely
        pthread_mutex_unlock(&enemiesMutex_);
        enemyMoveTimer_ = sf::Time::Zero;
    }

    for (auto it = bombas_.begin(); it != bombas_.end(); )
    {
        if (it->expirada())
        {
            int creador = it->getCreador();

            pushEvento(
                Evento::bombExplode(
                    it->getId(),
                    it->getCreador(),
                    it->getX(),
                    it->getY(),
                    it->getRadio()
                )
            );

            it = bombas_.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = explosiones_.begin();
        it != explosiones_.end(); )
    {
        if (it->expirada())
        {
            tablero_.removeOccupant(
                Position{it->getX(), it->getY()},
                it->getId()
            );

            it = explosiones_.erase(it);
        }
        else
        {
            ++it;
        }
    }

    updateGameState();
}

bool Engine::running() const
{
    return running_.load();
}

void* Engine::player_thread_process(void* arg)
{
    PlayerThreadArg* data = (PlayerThreadArg*) arg;
    Engine* engine = data -> engine;
    int playerId = data -> playerId;
    delete data;

    while (engine -> running()) {
        //Pendiente
        sf::sleep(sf::milliseconds(16));
    }

    return nullptr;
}

// logic_thread
void* Engine::logic_thread(void* arg) {
    Engine* engine = static_cast<Engine*>(arg);

    while (engine->running()) {
        std::optional<Evento> evento = engine->popEvento();

        for (Player& p : engine->jugadores_)
            p.actualizarInvencibilidad();

        if (evento.has_value())
            engine->procesarEvento(evento.value());
    }

    return nullptr;
}

void Engine::pushEvento(const Evento &evento)
{
    if (!running_.load())
        return;

    pthread_mutex_lock(&eventMutex_);

    if (!running_.load())
    {
        pthread_mutex_unlock(&eventMutex_);
        return;
    }

    listaEventos_.push(evento);

    pthread_cond_signal(&eventReady_);
    pthread_mutex_unlock(&eventMutex_);
}

std::optional<Evento> Engine::popEvento()
{
    pthread_mutex_lock(&eventMutex_);
    
    while (listaEventos_.empty() && running_.load())
        pthread_cond_wait(&eventReady_, &eventMutex_);

    if (!running_.load())
    {
        pthread_mutex_unlock(&eventMutex_);
        return std::nullopt;
    }
    else{
        Evento evento = listaEventos_.front();
        listaEventos_.pop();

        pthread_mutex_unlock(&eventMutex_);
        return evento;
    }
}

void Engine::procesarEvento(Evento& evento){
    switch (evento.tipo())
    {
        case EventType::PlayerMove:
            onPlayerMove(evento);
            break;
            
        case EventType::EnemyMove:
            onEnemyMove(evento);
            break;
            
        case EventType::PlayerPlaceBomb:
            onPlayerPlaceBomb(evento);
            break;

        case EventType::BombExplode:
            onBombExplode(evento);
            break;

        case EventType::TileDestroyed:
            onTileDestroyed(evento);
            break;
        
        
        case EventType::EnemyDeath:
            onEnemyDeath(evento);
            break;


        /*
            case EventType::PlayerDeath:
                onPlayerDeath(evento);
                break;


        case EventType::PlayerPickPowerUp:
            onPlayerPickPowerUp(evento);
            break;


        case EventType::ChainExplosion:
            onChainExplosion(evento);
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

void Engine::updateGameState()
{
    int alivePlayers = 0;

    for (const Player& p : jugadores_)
    {
        if (p.vida() > 0) ++alivePlayers;
    }

    if (alivePlayers <= 1) gameOver_ = true;
    if (roundTimer_ <= sf::Time::Zero) gameOver_ = true;
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

                for (const Occupant& occ : cell.occupants)
                {
                    if (occ.type == EntityType::Explosion)
                    {
                        snapshot.entities[y][x] = EntityType::Explosion;
                        break;
                    }

                    if (occ.type == EntityType::Bomb)
                    {
                        snapshot.entities[y][x] = EntityType::Bomb;
                    }
                }
            }
        }
    }

    // Copy player data
    for (const Player& p : jugadores_)
    {
        snapshot.players.push_back(PlayerData{
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
    snapshot.hud.gameOver  = gameOver_;
    snapshot.hud.numPlayers = jugadores_.size();

    return snapshot;
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

void Engine::onEnemyMove(Evento& evento)
{
    int enemyId = evento.autor();

    // Validar ID del enemigo
    if (enemyId < 0 || enemyId >= enemigos_.size())
        return;

    Enemigo& enemigo = enemigos_[enemyId];

    if (!enemigo.isAlive())
        return;

    int dx = evento.data().mover.dx;
    int dy = evento.data().mover.dy;

    Position oldPos{
        enemigo.posX(),
        enemigo.posY()
    };

    Position newPos{
        oldPos.x + dx,
        oldPos.y + dy
    };

    const BoardCell cell = tablero_.getCell(newPos);

    for (const Occupant& occ : cell.occupants)
    {
    if (occ.type == EntityType::Player1 ||
        occ.type == EntityType::Player2 ||
        occ.type == EntityType::Player3 ||
        occ.type == EntityType::Player4)
    {
        danioPlayer(occ.entityId);
        return;
    }
    }

    // Limites del tablero
    if (!tablero_.isWalkable(newPos))
        return;

    // Mover occupant en tablero
    bool moved = tablero_.moveOccupant(
        oldPos,
        newPos,
        enemyId
    );

    // Si no se pudo mover, no actualizamos la posicion
    if (!moved)
        return;

    //Actualizar posicion interna del enemigo
    enemigo.setPosition(
        newPos.x,
        newPos.y
    );
}

void Engine::moveEnemies() {
    for (int i = 0; i < (int)enemigos_.size(); i++) {

        if (!enemigos_[i].isAlive())
            continue;
            
        Directions direction;
        int num = rand() % 4;

        switch (num){
            case 0: direction = Directions::DOWN; break;
            case 1: direction = Directions::UP; break;
            case 2: direction = Directions::LEFT; break;
            case 3: direction = Directions::RIGHT; break;
            default: break;
        }

        // Enemy generates its own event
        pushEvento(enemigos_[i].generarEventoMov(direction));
    }
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

void Engine::onPlayerPlaceBomb(Evento& evento)
{
    int playerId = evento.autor();

    if (playerId < 0 || playerId >= jugadores_.size())
        return;

    Player& player = jugadores_[playerId];

    Position pos = player.getPosition();

    int bombaId = nextBombId_++;

    const BoardCell& cell = tablero_.getCell(pos);

    for (const Occupant& occ : cell.occupants)
    {
        if (occ.type == EntityType::Bomb)
            return;
    }

    bombas_.push_back(
        Bomba(
            bombaId,
            playerId,
            pos.x,
            pos.y,
            player.rangoExplosion(),
            sf::seconds(3.f)
        )
    );

    tablero_.addOccupant(
        pos,
        Occupant{
            EntityType::Bomb,
            bombaId
        }
    );
    
}

void Engine::onBombExplode(Evento& evento)
{
    int creador = evento.data().explosion.creador;

    jugadores_[creador].recuperarBomba();

    Position pos{
        evento.posicionX(),
        evento.posicionY()
    };

    int bombId = evento.autor();

    tablero_.removeOccupant(
        pos,
        bombId
    );

    createExplosion(pos,creador);

    int radio = evento.data().explosion.radio;
    explodeDirection(pos, creador,  1,  0, radio); // derecha
    explodeDirection(pos, creador, -1,  0, radio); // izquierda
    explodeDirection(pos, creador,  0, -1, radio); // arriba
    explodeDirection(pos, creador,  0,  1, radio); // abajo

}

void Engine::createExplosion(Position pos, int creador)
{
    int explosionId = nextExplosionId_++;

    explosiones_.push_back(
        Explosion(
            explosionId,
            pos.x,
            pos.y,
            sf::seconds(2.f) //Duracion de la explosion, pendiente de ajustar
        )
    );

    BoardCell cell = tablero_.getCell(pos);

    for (const Occupant& occ : cell.occupants)
    {
        if (occ.type == EntityType::Enemy)
        {
            Enemigo& enemigo = enemigos_[occ.entityId];

            auto deathEvent = enemigo.recibirDanio(jugadores_[creador]);

            if (deathEvent.has_value())
            {
                pushEvento(
                    deathEvent.value()
                );
            }
        }
    }

    tablero_.addOccupant(
        pos,
        Occupant{
            EntityType::Explosion,
            explosionId
        }
    );
}

void Engine::explodeDirection(
    Position origen,
    int creador,
    int dx,
    int dy,
    int radio
)
{
    for (int i = 1; i <= radio; i++)
    {
        Position p{
            origen.x + dx * i,
            origen.y + dy * i
        };

        if (p.x < 0 ||
            p.y < 0 ||
            p.x >= tablero_.getWidth() ||
            p.y >= tablero_.getHeight())
        {
            break;
        }

        BoardCell cell = tablero_.getCell(p);
        
        if (cell.terrainType == TileType::Wall)
        {
            break;
        }

        createExplosion(p, creador);

        if (cell.terrainType == TileType::Breakable)
        {
            
            pushEvento(
                Evento::tileDestroyed(
                    p.x,
                    p.y
                )
            );
            break;
        }
        
    }
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
}

void Engine::onEnemyDeath(Evento& evento)
{
    int enemyId = evento.objetivo();

    Position pos{
        evento.posicionX(),
        evento.posicionY()
    };

    tablero_.removeOccupant(
        pos,
        enemyId
    );
}

void Engine::handleInput(sf::Keyboard::Key key, int playerId)
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
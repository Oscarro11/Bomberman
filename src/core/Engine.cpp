#include "core/Engine.hpp"
#include "ecs/PlayerStats.hpp"
#include "utils/AssetsUtils.hpp"

std::vector<Player> Engine::jugadores_;

Engine::Engine(std::string tableroSource, std::vector<PlayerStats*> jugadores)
    : inputHandler_(this)
    , tablero_(tableroSource)
{
    pthread_mutex_init(&eventMutex_, NULL);
    pthread_mutex_init(&inputMutex_, NULL);
    pthread_cond_init(&eventReady_, NULL);

    auto playersSpawn = tablero_.getSpawnPlayers();
    
    for (int i=0; i < jugadores.size(); i++)
    {
        PlayerStats* info = jugadores.at(i);

        Player player = Player(i, 3, info -> maxBombas, info -> velocidad, playersSpawn[i].position.x, playersSpawn[i].position.y);
        this -> jugadores_.push_back(player);

        switch (i)
        {
        case 0: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player1, i}); break;
        case 1: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player2, i}); break;
        case 2: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player3, i}); break;
        case 3: tablero_.addOccupant(Position{player.posX(), player.posY()}, Occupant{EntityType::Player4, i}); break;
        default: break;}

        //Se crea un hilo por cada jugador
        /*
        pthread_t thread;
        PlayerThreadArg* arg = new PlayerThreadArg{this, i};
        
        pthread_create(&thread, NULL, player_thread_process, arg);
        threads.push_back(thread);
        */
    };
    

    //Hilos de procesamiento de eventos y de inputs
    pthread_create(&logicThread_, NULL, logic_thread, (void*) this);
}

Engine::~Engine()
{
    pthread_mutex_lock(&eventMutex_);

    running_ = false;

    pthread_cond_broadcast(&eventReady_);

    pthread_mutex_unlock(&eventMutex_);

    for (const pthread_t& thread : threads)
    {
        pthread_join(thread, NULL);
    }
    

    pthread_join(logicThread_, NULL);

    pthread_mutex_destroy(&eventMutex_);
    pthread_mutex_destroy(&inputMutex_);
    pthread_cond_destroy(&eventReady_);
}

bool Engine::running() const
{
    return this -> running_;
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

    //Unicamente se usa para procesar eventos, teniendo pop una forma interna de dormirse
    while (engine -> running()) {
        std::optional<Evento> evento = engine -> popEvento();       // locks + unlocks internally
        if (evento.has_value())
        {
            engine -> procesarEvento(evento.value());             // called without mutex held
        }
    }

    return nullptr;
}

void Engine::pushEvento(const Evento &evento)
{
    if (!running_)
        return;

    pthread_mutex_lock(&eventMutex_);

    if (!running_)
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
    
    while (listaEventos_.empty() && running_)
        pthread_cond_wait(&eventReady_, &eventMutex_);

    if (!running_)
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

        /*
        case EventType::PlayerPlaceBomb:
            onPlayerPlaceBomb(e);
            break;

        case EventType::PlayerDeath:
            onPlayerDeath(e);
            break;

        case EventType::PlayerPickPowerUp:
            onPlayerPickPowerUp(e);
            break;

        case EventType::BombExplode:
            onBombExplode(e);
            break;

        case EventType::ChainExplosion:
            onChainExplosion(e);
            break;

        case EventType::EnemyMove:
            onEnemyMove(e);
            break;

        case EventType::EnemyDeath:
            onEnemyDeath(e);
            break;

        case EventType::TileDestroyed:
            onTileDestroyed(e);
            break;

        case EventType::GameOver:
            onGameOver(e);
            break;

        case EventType::RoundStart:
            onRoundStart(e);
            break;
            */
    }
}

RenderSnapshot Engine::makeRenderSnapshot()
{
    RenderSnapshot snapshot;

    //pthread_mutex_lock(&boardMutex_);

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

    //pthread_mutex_unlock(&boardMutex_);

    // Copy player data
    for (const Player& p : jugadores_)
    {
        snapshot.players.push_back(PlayerData{
            static_cast<int>(p.id()),
            static_cast<int>(p.vida()),
            static_cast<int>(p.maxBombas()),
            static_cast<int>(p.restBombas()),
            static_cast<int>(p.rangoExplosion()),
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
    //TODO: remake this method taking into account new structure of Tablero
    /*
    Player& player = jugadores_[evento.autor()];

    int dx = evento.data().mover.dx;
    int dy = evento.data().mover.dy;

    int oldX = player.posX();
    int oldY = player();

    int newX = oldX + dx;
    int newY = oldY + dy;

    // Limites del tablero
    if (newX < 0 || newY < 0 ||
        newX >= tablero_.getWidth() ||
        newY >= tablero_.getHeight())
    {
        return;
    }

    CellContent destino = tablero_.getCell({newX, newY});

    // Colisiones
    switch (destino.type)
    {
        case Wall:
        case DestructibleWall:
        case Bomb:
            return;

        default:
            break;
    }

    // Liberar celda anterior
    tablero_.setCell(
        Floor,
        -1,
        {oldX, oldY}
    );

    // Actualizar posicion
    player.setPosition(newX, newY);

    // Actualizar tablero
    BoardElement tipoJugador =
        static_cast<BoardElement>(Player1 + player.id());

    tablero_.setCell(
        tipoJugador,
        player.id(),
        {newX, newY}
    );

    printf(
        "Jugador %i se movio a (%i, %i)\n",
        player.id(),
        newX,
        newY
    );
    */
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
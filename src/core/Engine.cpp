#include "core/Engine.hpp"

std::vector<Player> Engine::jugadores_;

Engine::Engine(std::string tableroSource, std::vector<PlayerInfo> jugadores)
    : running_(true), inputHandler_(this)
{
    tablero_.cargar(tableroSource);

    pthread_mutex_init(&eventMutex_, NULL);
    pthread_mutex_init(&inputMutex_, NULL);
    pthread_cond_init(&eventReady_, NULL);

    for (int i=0; i < jugadores.size(); i++)
    {
        PlayerInfo info = jugadores.at(i);

        //Se crea un hilo por cada jugador
        Player player = Player(i, info.vida, info.maxBombas, info.velocidad, 0, 0);
        this -> jugadores_.push_back(player);

        pthread_t thread;
        PlayerThreadArg* arg = new PlayerThreadArg{this, i};
        
        pthread_create(&thread, NULL, player_thread_process, arg);
        threads.push_back(thread);
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

        case EventType::PlayerPlaceBomb:
            onPlayerPlaceBomb(evento);
            break;

        case EventType::BombExplode:
            onBombExplode(evento);
            break;

        /*

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

void Engine::onBombExplode(Evento& evento)
{
    int cx = evento.posicionX();
    int cy = evento.posicionY();
    unsigned int radio = evento.data().explosion.radio;

    auto it = bombToPlayer_.find(evento.autor());
    Player* atacante = (it != bombToPlayer_.end()) ? &jugadores_[it->second] : nullptr;

    // Las 4 direcciones: derecha, izquierda, abajo, arriba
    const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    // Celda central también recibe explosión
    for (auto& jugador : jugadores_) {
        if ((int)jugador.posX() == cx && (int)jugador.posY() == cy && atacante) {
            auto muerte = jugador.recibirDanio(*atacante);
            if (muerte.has_value()) pushEvento(muerte.value());
        }
    }

    for (auto& dir : dirs) {
        for (unsigned int paso = 1; paso <= radio; paso++) {
            int nx = cx + dir[0] * (int)paso;
            int ny = cy + dir[1] * (int)paso;

            char tile = tablero_.getTile(nx, ny);

            if (tile == '#') break;

            for (auto& jugador : jugadores_) {
                if ((int)jugador.posX() == nx && (int)jugador.posY() == ny && atacante) {
                    auto muerte = jugador.recibirDanio(*atacante);
                    if (muerte.has_value()) pushEvento(muerte.value());
                }
            }

            if (tile == '+') {
                tablero_.setTile(nx, ny, '.');
                pushEvento(Evento::tileDestroyed(nx, ny));
                break;
            }
        }
    }

    bombToPlayer_.erase(evento.autor());
    printf("Bomba %i exploto en (%i,%i) con radio %u\n",
        evento.autor(), cx, cy, radio);
}

void Engine::onPlayerPlaceBomb(Evento& evento)
{
    Player& player = jugadores_[evento.autor()];

    // Intenta tomar una ranura del semáforo; si el jugador ya alcanzó su límite, no hace nada
    if (sem_trywait(player.getSem()) != 0)
        return;

    static int bombIdCounter = 0;
    int bombId = bombIdCounter++;
    bombToPlayer_[bombId] = evento.autor();

    auto bomba = std::make_unique<Bomba>(
        bombId,
        evento.posicionX(),
        evento.posicionY(),
        player.rangoExplosion(),
        3000,
        this,
        player.getSem()
    );
    bomba->iniciar();
    bombas_.push_back(std::move(bomba));

    printf("Jugador %i coloco bomba %i en (%i,%i)\n",
        evento.autor(), bombId, evento.posicionX(), evento.posicionY());
}

void Engine::onPlayerMove(Evento &evento)
{
    Player& player = jugadores_[evento.autor()];

    //Pendiente de implementarse
    printf("El jugador con id %i se movera segun los siguientes cambios: %i, %i\n", player.id(), evento.data().mover.dx, evento.data().mover.dy);
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
    else if (key == km.bomb)  pushEvento(p.colocarBomba());
}
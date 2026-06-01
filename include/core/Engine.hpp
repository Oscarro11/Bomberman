#pragma once

#include <vector>
#include <queue>
#include <string>
#include <atomic>
#include <pthread.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "core/IEngine.hpp"
#include "entities/Tablero.hpp"
#include "entities/Player.hpp"
#include "entities/Bomba.hpp"
#include "entities/Enemigo.hpp"
#include "entities/Explosion.hpp"
#include "entities/PowerUp.hpp"
#include "input/InputHandler.hpp"
#include "rendering/RenderSnapshot.hpp"

struct PlayerStats;

class Engine : public IEngine
{
    private:
        std::atomic<bool> running_;
        Tablero tablero_;
        std::vector<Player> jugadores_;
        std::vector<Enemigo> enemigos_;
        std::vector<Bomba> bombas_;
        std::queue<Evento> listaEventos_;
        std::vector<Explosion> explosiones_;
        std::vector<PowerUp> powerUps_;

        int nextExplosionId_ = 0;
        int nextBombId_ = 0;

        sf::Time roundTimer_;
        bool gameOver_;
        bool singlePlayer_;

        sf::Time enemyMoveTimer_ = sf::Time::Zero;
        static constexpr float ENEMY_MOVE_INTERVAL = 1.f;   // seconds between moves

        /*
        */

        pthread_t logicThread_;

        //Clase dedica a leer inputs, tiene su propio hilo
        InputHandler inputHandler_;
        std::vector<pthread_t> playerThreads_;

        //Mutex para proteger el queue de eventos al ingresar uno nuevo
        mutable pthread_mutex_t eventMutex_;

        //Mutex para proteger el acceso a jugadores cuando un nuevo input se lee.
        //Hay que revisar si es necesario
        mutable pthread_mutex_t inputMutex_;

        //Mutex para proteger el acceso a la lista de enemigos
        mutable pthread_mutex_t enemiesMutex_;

        //Condicion que se usa para indicar al hilo de procesamiento que hay un evento disponible
        pthread_cond_t eventReady_;

        std::vector<pthread_t> threads;
        static void* player_thread_process(void* arg);
        static void* logic_thread(void* arg);

        void updateGameState();

        std::optional<Evento> popEvento();
        void procesarEvento(Evento& evento);
        void onPlayerMove(Evento& evento);
        void onEnemyMove(Evento& evento);
        void onPlayerDeath(Evento& evento);
        void onPlayerPlaceBomb(Evento& evento);
        void onBombExplode(Evento& evento);
        void createExplosion(Position pos, int autor);
        void explodeDirection(Position origen,int creador,int dx,int dy,int radio);
        void moveEnemies();
        void danioPlayer(int playerId);
        void onTileDestroyed(Evento& evento);
        void onEnemyDeath(Evento& evento);
        void onChainExplosion(Evento& evento);
        void onPlayerPickPowerUp(Evento& evento);

        //void updateBombs(sf::Time dt);
        //void updateExplosions(sf::Time dt);
        //void updatePowerUps();

    public:
        Engine(std::string tableroSource, std::vector<PlayerStats>& jugadores);
        virtual ~Engine();

        void start();

        void update(sf::Time dt);

        int numPlayers() const override{return jugadores_.size();}

        //Deberia cambiarse, o revisar de hacer una interfaz para que solo inputHandler y Engine puedan acceder
        Player& getPlayer(int id) {return jugadores_.at(id);}

        bool running() const override;

        void handleInput(sf::Keyboard::Key key, int playerId) override;
        void pushEvento(const Evento& evento) override;
        RenderSnapshot makeRenderSnapshot();
};

struct EnemyThreadArg{
    Engine* engine;
    int     enemyId;
};

//Estructura para crear el hilo de cada jugador
struct PlayerThreadArg
{
    Engine* engine;
    int playerId;
};
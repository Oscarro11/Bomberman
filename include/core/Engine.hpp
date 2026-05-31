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
#include "entities/Enemigo.hpp"
#include "input/InputHandler.hpp"
#include "rendering/RenderSnapshot.hpp"
#include "systems/EventBus.hpp"

struct PlayerStats;

class Engine : public IEngine
{
    private:
        EventBus eventBus_;

        std::atomic<bool> running_;
        Tablero tablero_;
        std::vector<Player> jugadores_;
        std::vector<Enemigo> enemigos_;
        std::queue<Evento> listaEventos_;

        sf::Time roundTimer_;
        bool gameOver_;

        sf::Time enemyMoveTimer_ = sf::Time::Zero;
        static constexpr float ENEMY_MOVE_INTERVAL = 0.8f;   // seconds between moves

        /*
        vector<Bomba> listaBombas;
        vector<PowerUp> listaPowerUps;
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

        void onPlayerMove(Evento& evento);
        void onEnemyMove(Evento& evento);
        void onPlayerDeath(Evento& evento);
        void moveEnemies();
        void danioPlayer(int playerId);

        //void updateBombs(sf::Time dt);
        //void updateExplosions(sf::Time dt);
        //void updatePowerUps();

    public:
        Engine(std::string tableroSource, std::vector<PlayerStats>& jugadores);
        virtual ~Engine();

        void start();

        void update(sf::Time dt);

        int numPlayers() const override{return jugadores_.size();}

        void pushEvento(const Evento& e);
        std::optional<Evento> popEvento();
        void procesarEvento(Evento& evento);

        //Deberia cambiarse, o revisar de hacer una interfaz para que solo inputHandler y Engine puedan acceder
        Player& getPlayer(int id) {return jugadores_.at(id);}

        bool running() const override;

        void handleInput(sf::Keyboard::Key key, int playerId) override;
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
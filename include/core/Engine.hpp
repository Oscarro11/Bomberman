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

struct PlayerStats;

class Engine : public IEngine
{
    private:
        std::atomic<bool> running_;
        Tablero tablero_;
        std::vector<Player> jugadores_;
        std::vector<Enemigo> enemigos_;
        std::queue<Evento> listaEventos_;

        sf::Time roundTimer_;
        std::atomic<bool> paused_ = false;
        bool gameOver_;

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

        void pause();
        void resume();
        bool isPaused() const { return paused_.load(); }

        int numPlayers() const override{return jugadores_.size();}

        //Deberia cambiarse, o revisar de hacer una interfaz para que solo inputHandler y Engine puedan acceder
        Player& getPlayer(int id) {return jugadores_.at(id);}

        bool running() const override;
        inline bool paused() const {return paused_;};

        void handleInput(sf::Keyboard::Key key, int playerId) override;
        void pushEvento(const Evento& evento) override;
        RenderSnapshot makeRenderSnapshot();
};

//Estructura para crear el hilo de cada jugador
struct PlayerThreadArg
{
    Engine* engine;
    int playerId;
};
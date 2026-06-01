#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <string>
#include <atomic>
#include <unordered_map>
#include <pthread.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "ecs/Evento.hpp"
#include "entities/Player.hpp"
#include "entities/Bomba.hpp"
#include "core/IEngine.hpp"
#include "core/Tablero.hpp"
#include "core/RenderSnapshot.hpp"
#include "input/InputHandler.hpp"

//Estructura de datos usada en Vista, para trasladar la info del menu principal
//a las instancias de los jugadores
struct PlayerInfo
{
    unsigned int vida;
    unsigned int maxBombas;
    unsigned int rangoExplosion;
    double velocidad;
};

class Engine : public IEngine
{
    private:
        std::atomic<bool> running_;
        Tablero tablero_;
        static std::vector<Player> jugadores_;
        std::queue<Evento> listaEventos_;
        std::vector<std::unique_ptr<Bomba>> bombas_;
        std::unordered_map<int,int> bombToPlayer_;

        pthread_t logicThread_;

        //Clase dedica a leer inputs, tiene su propio hilo
        InputHandler inputHandler_;
        std::vector<pthread_t> playerThreads_;

        //Mutex para proteger el queue de eventos al ingresar uno nuevo
        pthread_mutex_t eventMutex_;

        //Mutex para proteger el acceso a jugadores cuando un nuevo input se lee.
        //Hay que revisar si es necesario
        pthread_mutex_t inputMutex_;

        //Condicion que se usa para indicar al hilo de procesamiento que hay un evento disponible
        pthread_cond_t eventReady_;

        std::vector<pthread_t> threads;
        static void* player_thread_process(void* arg);
        static void* logic_thread(void* arg);

        std::optional<Evento> popEvento();
        void procesarEvento(Evento& evento);
        void onPlayerMove(Evento& evento);
        void onPlayerPlaceBomb(Evento& evento);
        void onBombExplode(Evento& evento);

    public:
        Engine(std::string tableroSource, std::vector<PlayerInfo> jugadores);
        virtual ~Engine();

        int numPlayers() const override{return jugadores_.size();}

        //Deberia cambiarse, o revisar de hacer una interfaz para que solo inputHandler y Engine puedan acceder
        Player& getPlayer(int id) {return jugadores_.at(id);}

        bool running() const override;
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
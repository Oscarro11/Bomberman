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
#include "systems/EventBus.hpp"
#include "systems/EnemySystem.hpp"
#include "systems/BombSystem.hpp"

struct PlayerStats;

enum class MatchState
{
    Preparing,
    Playing,
    Paused,
    WaitingForGameOverConfirmation,
    Finished
};

class Engine : public IEngine
{
    private:
        MatchState state_;

        EventBus eventBus_;
        EnemySystem enemiesSystem_;
        BombSystem bombsSystem_;

        Tablero tablero_;
        std::vector<Player> jugadores_;
        std::vector<Enemigo> enemigos_;
        std::vector<Bomba> bombas_;
        std::vector<Explosion> explosiones_;
        std::vector<PowerUp> powerUps_;

        bool singlePlayer_ = true;

        sf::Time roundTimer_;
        pthread_t logicThread_;
        pthread_t enemyThread_;

        //Clase dedica a leer inputs, tiene su propio hilo
        InputHandler inputHandler_;

        //Mutex para proteger el acceso a jugadores cuando un nuevo input se lee.
        //Hay que revisar si es necesario
        mutable pthread_mutex_t inputMutex_;
        
        static void* enemy_thread(void* arg);
        static void* logic_thread(void* arg);

        void updatePlayersState();
        void updateGameState();

        void onPlayerMove(Evento& evento);
        //void onPlayerDeath(Evento& evento);
        void onPlayerPlaceBomb(Evento& evento);
        void onBombExplode(Evento& evento);

        void danioPlayer(int playerId);
        void onTileDestroyed(Evento& evento);
        void onEnemyDeath(Evento& evento);
        void onChainExplosion(Evento& evento);
        void onPlayerPickPowerUp(Evento& evento);

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

        std::string getWinnerName() const;
        int         getWinnerId()   const;

        void handleInput(sf::Keyboard::Key key, int playerId) override;
        RenderSnapshot makeRenderSnapshot();

        inline MatchState state() const {return state_;};
        inline bool isPlaying() const {return state_ == MatchState::Playing;};
        inline bool isFinished() const {return state_ == MatchState::Finished;};

        void confirmGameOver();
};

struct EnemyThreadArg{
    Engine* engine;
    int     enemyId;
};
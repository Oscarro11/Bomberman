#include <vector>
#include <SFML/Graphics.hpp>
#include <pthread.h>

#include "entities/Enemigo.hpp"
#include "entities/Tablero.hpp"
#include "entities/Explosion.hpp"
#include "systems/EventBus.hpp"

class EnemySystem {
    public:
        EnemySystem(std::vector<Enemigo>& enemies, std::vector<Explosion>& explosions, Tablero& tablero, EventBus& bus);
        ~EnemySystem();

        void update(sf::Time dt);
        std::optional<int> processMove(const Evento& evento);
        
        void killEnemy(int enemyId);
        bool hasLivingEnemies();

    private:
        std::vector<Enemigo>& enemies_;
        std::vector<Explosion>& explosions_;
        EventBus& bus_;
        Tablero& tablero_;
        
        pthread_mutex_t enemyMutex_;
        sf::Time timer_;
        sf::Time ENEMY_MOVE_INTERVAL = sf::seconds(0.8f);
};
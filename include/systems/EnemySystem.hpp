#include <vector>
#include <SFML/Graphics.hpp>

#include "entities/Enemigo.hpp"
#include "entities/Tablero.hpp"
#include "systems/EventBus.hpp"

class EnemySystem {
    public:
        EnemySystem(std::vector<Enemigo>& enemies, Tablero& tablero, EventBus& bus);

        void update(sf::Time dt);
        std::optional<int> processMove(const Evento& evento);

    private:
        std::vector<Enemigo>& enemies_;
        EventBus& bus_;
        Tablero& tablero_;
        
        sf::Time timer_;
        sf::Time ENEMY_MOVE_INTERVAL = sf::seconds(0.8f);
};
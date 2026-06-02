#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "entities/Tablero.hpp"
#include "entities/Bomba.hpp"
#include "entities/Explosion.hpp"
#include "entities/Enemigo.hpp"
#include "entities/Player.hpp"

#include "systems/EventBus.hpp"

class BombSystem {
    public:
        BombSystem(
            std::vector<Player>& players,
            std::vector<Bomba>& bombs,
            std::vector<Explosion>& explosions,
            std::vector<Enemigo>& enemies,
            Tablero& tablero,
            EventBus& bus);

        void update(sf::Time dt);

        bool placeBomb(int creatorId, int x, int y, int range);
        void explodeBomb(const Evento& evento);
        void createExplosion(Position pos, int creador);
        void explodeDirection(Position origen, int creador, int dx, int dy, int radio);

    private:
        std::vector<Player>& players_;
        std::vector<Bomba>& bombs_;
        std::vector<Explosion>& explosions_;
        std::vector<Enemigo>& enemies_;

        Tablero& tablero_;
        EventBus& bus_;

        int nextBombId_ = 0;
        int nextExplosionId_ = 0;
};


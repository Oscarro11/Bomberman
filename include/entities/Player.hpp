#pragma once

#include <optional>

#include "entities/Personaje.hpp"
#include "utils/Position.hpp"

//Forward declaration
enum PowerUpType : int;

class Player : public Personaje
{
    private:
        unsigned int maxBombas_;
        unsigned int restBombas_;
        unsigned int rangoExplosion_;
        double velocidad_;
        unsigned int spawnPointX_;
        unsigned int spawnPointY_; 

    public:
        Player(unsigned int id, unsigned int vida, unsigned int maxBombas, double velocidad, int spawnPointX, int spawnPointY);
        virtual ~Player() = default;

        Evento generarEventoMov(Directions direction) override;
        std::optional<Evento> recibirDanio(const Personaje& atacante) override;

        std::optional<Evento> colocarBomba();
        void actualizarStat(PowerUpType tipo, int cantidad);

        int posX() const { return posX_; }
        int posY() const { return posY_; }

        Position position() const
        {
            return Position{
                static_cast<unsigned int>(posX_),
                static_cast<unsigned int>(posY_)
            };
        }

        void setPosition(int x, int y)
        {
            posX_ = x;
            posY_ = y;
        }

        unsigned int maxBombas() const {return maxBombas_;};
        unsigned int restBombas() const {return restBombas_;};
        unsigned int rangoExplosion() const {return rangoExplosion_;};
        double velocidad() const {return velocidad_;};
        unsigned int spawnPointX() const {return spawnPointX_;};
        unsigned int spawnPointY() const {return spawnPointY_;};
};

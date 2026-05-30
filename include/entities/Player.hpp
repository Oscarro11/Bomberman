#pragma once

#include <optional>

#include "entities/Personaje.hpp"
#include "utils/Position.hpp"

//Forward declaration
enum PowerUpType : int;

class Player : public Personaje
{
    private:
        int maxBombas_;
        int restBombas_;
        int rangoExplosion_;
        int velocidad_;
        int spawnPointX_;
        int spawnPointY_; 

    public:
        Player(int id, int vida, int maxBombas, int rangoExplosion, int velocidad, int spawnPointX, int spawnPointY);
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
                posX_, posY_
            };
        }

        void setPosition(int x, int y)
        {
            posX_ = x;
            posY_ = y;
        }

        int maxBombas() const {return maxBombas_;};
        int restBombas() const {return restBombas_;};
        int rangoExplosion() const {return rangoExplosion_;};
        int velocidad() const {return velocidad_;};
        int spawnPointX() const {return spawnPointX_;};
        int spawnPointY() const {return spawnPointY_;};
};

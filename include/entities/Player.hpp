#pragma once

#include <optional>

#include "entities/Personaje.hpp"

//Forward declaration
enum PowerUpType : int;

class Player : public Personaje
{
    private:
        unsigned int maxBombas_;
        unsigned int rangoExplosion_;
        double velocidad_;
        unsigned int spawnPointX_;
        unsigned int spawnPointY_; 

    public:
        Player(int id, unsigned int vida, unsigned int maxBombas, double velocidad, int spawnPointX, int spawnPointY);
        virtual ~Player() = default;

        Evento generarEventoMov(Directions direction) override;
        std::optional<Evento> recibirDanio(const Personaje& atacante) override;

        Evento colocarBomba();
        void actualizarStat(PowerUpType tipo, int cantidad);
};

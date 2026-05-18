#pragma once
#include "entities/Personaje.hpp"
#include "entities/PowerUp.hpp"

class Player : public Personaje
{
    private:
        unsigned int maxBombas;
        unsigned int rangoExplosion;
        double velocidad;
        unsigned int spawnPointX;
        unsigned int spawnPointY; 

    public:
        Player(int id, unsigned int vida, unsigned int maxBombas, double velocidad, int spawnPointX, int spawnPointY);
        virtual ~Player() = default;

        Evento generarEventoMov();
        void recibirDanio();

        Evento colocarBomba();
        void actualizarStat(PowerUpType tipo, int cantidad);
};

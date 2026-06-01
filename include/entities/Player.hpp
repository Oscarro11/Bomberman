#pragma once
#include <semaphore.h>
#include "entities/Personaje.hpp"
#include "entities/PowerUp.hpp"

class Player : public Personaje
{
    private:
        unsigned int maxBombas_;
        unsigned int rangoExplosion_;
        double velocidad_;
        unsigned int spawnPointX_;
        unsigned int spawnPointY_;
        sem_t semBombas_;

    public:
        Player(int id, unsigned int vida, unsigned int maxBombas, double velocidad, int spawnPointX, int spawnPointY);
        virtual ~Player();

        Evento generarEventoMov(Directions direction) override;
        std::optional<Evento> recibirDanio(const Personaje& atacante) override;

        Evento colocarBomba();
        void actualizarStat(PowerUpType tipo, int cantidad);

        sem_t* getSem() { return &semBombas_; }
        unsigned int rangoExplosion() const { return rangoExplosion_; }
};

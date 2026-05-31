#pragma once

#include "entities/Personaje.hpp"
#include "ecs/Evento.hpp"

class Enemigo : public Personaje
{
    private:
        int tipo_;
        int alcance_;

    public:
        Enemigo(int id, int tipo, int alcance, int x, int y);
        Evento generarEventoMov(Directions direction) override;
        std::optional<Evento> recibirDanio(const Personaje& atacante) override;
};
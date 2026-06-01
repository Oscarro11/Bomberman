#include <math.h>
#include "entities/Enemigo.hpp"

Enemigo::Enemigo(int id, int tipo, int alcance, int x, int y)
{
    this -> id_ = id;
    this -> tipo_ = tipo;
    this -> alcance_ = alcance;
    this -> posX_ = x;
    this -> posY_ = y;
    this -> vida_ = 1;

    this -> alive_ = true;
    this -> recibioDanio_ = false;
}

Evento Enemigo::generarEventoMov(Directions direction) {
    int dx = 0, dy = 0;

    switch (direction) {
        case Directions::UP: dy = -1; break;
        case Directions::DOWN: dy =  1; break;
        case Directions::LEFT: dx = -1; break;
        case Directions::RIGHT: dx =  1; break;
    }

    return Evento::enemyMove(id_, posX_, posY_, dx, dy);
}

std::optional<Evento> Enemigo::recibirDanio(const Personaje& atacante)
{
    vida_--;

    if (vida_ <= 0)
    {
        alive_ = false;

        return Evento::enemyDeath(
            id_,
            atacante.id(),
            posX_,
            posY_
        );
    }

    return std::nullopt;
}
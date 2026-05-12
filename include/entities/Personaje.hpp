#pragma once
#include "ecs/Evento.hpp"

class Personaje
{
protected:
    int id;
    unsigned int posX;
    unsigned int posY;
    unsigned int vida;

public:
    virtual ~Personaje() = default;

    virtual Evento generarEventoMov() = 0;
    virtual void recibirDanio() = 0;
};
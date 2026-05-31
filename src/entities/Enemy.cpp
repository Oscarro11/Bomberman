#include "entities/Enemigo.hpp"

Enemigo::Enemigo(int tipo, int alcance, int x, int y)
{
    this->tipo_ = tipo;
    this->recibioDano_ = false;
    this->alcance_ = alcance;
    this->posX_ = x;
    this->posY_ = y;
}

int Enemigo::getX()
{
    return posX_;
}

int Enemigo::getY()
{
    return posY_;
}

void Enemigo::setPosition(int x, int y)
{
    posX_ = x;
    posY_ = y;
}
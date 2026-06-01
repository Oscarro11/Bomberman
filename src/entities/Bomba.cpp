#include "entities/Bomba.hpp"

Bomba::Bomba(int id, int creador, int x, int y, int radio, sf::Time duracion)
{
    this->id_ = id;
    this->creador = creador;
    this->posX = x;
    this->posY = y;
    this->radio = radio;
    this->duracion = duracion;
    this->reloj_.restart();
}

int Bomba::getId() const
{
    return id_;
}

int Bomba::getCreador()
{
    return creador;
}

int Bomba::getRadio()
{
    return radio;
}

int Bomba::getX()
{
    return posX;
}

int Bomba::getY()
{
    return posY;
}

bool Bomba::expirada() const
{
    return reloj_.getElapsedTime() >= duracion;
}
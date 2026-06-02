#include "entities/Explosion.hpp"

Explosion::Explosion(int id,int posX,int posY,sf::Time duracion, int creatorId)
{
    this->id_ = id;
    this->creatorId_ = creatorId;
    this->posX = posX;
    this->posY = posY;
    this->duracion_ = duracion;

    reloj_.restart();
}

int Explosion::getId() const
{
    return id_;
}

int Explosion::getX()
{
    return posX;
}

int Explosion::getY()
{
    return posY;
}

int Explosion::getCreatorId() const
{
    return creatorId_;
}

bool Explosion::expirada() const
{
    return reloj_.getElapsedTime() >= duracion_;
}
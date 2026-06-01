#include "entities/PowerUp.hpp"

PowerUp::PowerUp(PowerUpType tipo,int posX,int posY)
{
    this->tipo_ = tipo;
    this->posX_ = posX;
    this->posY_ = posY;
}

PowerUpType PowerUp::tipo() const
{
    return tipo_;
}

int PowerUp::posX() const
{
    return posX_;
}

int PowerUp::posY() const
{
    return posY_;
}
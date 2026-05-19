#include "entities/PowerUp.hpp"

PowerUp::PowerUp(PowerUpType tipo, int posX, int posY)
{
    this -> tipo_ = tipo;
    this -> posX_ = posX;
    this -> posY_ = posY;
}
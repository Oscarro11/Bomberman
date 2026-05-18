#include "entities/PowerUp.hpp"

PowerUp::PowerUp(PowerUpType tipo, int posX, int posY)
{
    this -> tipo = tipo;
    this -> posX = posX;
    this -> posY = posY;
}
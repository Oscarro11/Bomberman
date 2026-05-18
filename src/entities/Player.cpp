#include "entities/Player.hpp"
#include "entities/PowerUp.hpp"

Player::Player(int id, unsigned int vida, unsigned int maxBombas, double velocidad, int spawnPointX, int spawnPointY)
{
    this -> id = id;
    this -> vida = vida;
    this -> maxBombas = maxBombas;
    this -> velocidad = velocidad;

    this -> spawnPointX = spawnPointX;
    this -> posX = spawnPointX;
    this -> spawnPointY = spawnPointY;
    this -> posY = spawnPointY;
}

Evento Player::colocarBomba()
{
    return Evento::playerPlaceBomb(id, posX, posY);
}

void Player::actualizarStat(PowerUpType tipo, int cantidad)
{
    switch (tipo)
    {
        case maxBomb: this -> maxBombas += cantidad;
        case explosionRange: this -> rangoExplosion += cantidad;
        case speed: this -> velocidad += cantidad;
        default: break;
    }
}

void Player::recibirDanio()
{
    vida--;
}

Evento Player::generarEventoMov(){
    return Evento::playerMove(id, posX, posY, 0, 0);
}
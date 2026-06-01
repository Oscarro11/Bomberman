#include "entities/Player.hpp"
#include "entities/PowerUp.hpp"

Player::Player(int id, unsigned int vida, unsigned int maxBombas, double velocidad, int spawnPointX, int spawnPointY)
{
    this -> id_ = id;
    this -> vida_ = vida;
    this -> maxBombas_ = maxBombas;
    this -> rangoExplosion_ = 1;
    this -> velocidad_ = velocidad;

    this -> spawnPointX_ = spawnPointX;
    this -> posX_ = spawnPointX;
    this -> spawnPointY_ = spawnPointY;
    this -> posY_ = spawnPointY;

    sem_init(&semBombas_, 0, maxBombas);
}

Player::~Player()
{
    sem_destroy(&semBombas_);
}

std::optional<Evento> Player::recibirDanio(const Personaje& atacante)
{
    vida_--;

    //Si el jugador muere, se procesa con un evento en el hilo principal. De lo contrario, no se regresa nada
    if (vida_ == 0)
    {
        return Evento::playerDeath(atacante.id(), id(), posX_, posY_);
    }
    
    return std::nullopt;
}

Evento Player::generarEventoMov(Directions direction){
    switch (direction)
    {
        case Directions::UP : return Evento::playerMove(id_, posX_, posY_, 0, 1);
        case Directions::DOWN : return Evento::playerMove(id_, posX_, posY_, 0, -1);
        case Directions::LEFT : return Evento::playerMove(id_, posX_, posY_, 1, 0);
        case Directions::RIGHT : return Evento::playerMove(id_, posX_, posY_, -1, 0);
        default : return Evento::playerMove(id_, posX_, posY_, 0, 0);
    }
}

Evento Player::colocarBomba()
{
    return Evento::playerPlaceBomb(id_, posX_, posY_);
}

void Player::actualizarStat(PowerUpType tipo, int cantidad)
{
    switch (tipo)
    {
        case maxBomb:        this->maxBombas_ += cantidad;      break;
        case explosionRange: this->rangoExplosion_ += cantidad; break;
        case speed:          this->velocidad_ += cantidad;      break;
        default: break;
    }
}
#include "entities/Player.hpp"
#include "entities/PowerUp.hpp"

Player::Player(int id, std::string nombre, int vida, int maxBombas, int rangoExplosion, int spawnPointX, int spawnPointY)
{
    this -> id_ = id;
    this -> nombre_ = nombre;
    this -> puntaje_ = 0;

    this -> vida_ = vida;
    this -> maxBombas_ = maxBombas;
    this -> restBombas_ = maxBombas;
    this -> rangoExplosion_ = rangoExplosion;

    this -> spawnPointX_ = spawnPointX;
    this -> posX_ = spawnPointX;
    this -> spawnPointY_ = spawnPointY;
    this -> posY_ = spawnPointY;
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
        case Directions::UP : return Evento::playerMove(id_, posX_, posY_, 0, -1);
        case Directions::DOWN : return Evento::playerMove(id_, posX_, posY_, 0, 1);
        case Directions::LEFT : return Evento::playerMove(id_, posX_, posY_, -1, 0);
        case Directions::RIGHT : return Evento::playerMove(id_, posX_, posY_, 1, 0);
        default : return Evento::playerMove(id_, posX_, posY_, 0, 0);
    }
}

std::optional<Evento> Player::colocarBomba()
{
    if (restBombas_ > 0)
    {
        restBombas_--;
        return Evento::playerPlaceBomb(id_, posX_, posY_);
    }
    
    return std::nullopt;
}

void Player::actualizarStat(PowerUpType tipo, int cantidad)
{
    switch (tipo)
    {
        case maxBomb: 
            this -> maxBombas_ += cantidad;
            this -> restBombas_ += cantidad;
            break;
        case explosionRange: 
            this -> rangoExplosion_ += cantidad;
            break;
        case life: 
            this -> vida_ += cantidad;
            break;
        default: break;
    }
}

void Player::recuperarBomba()
{
    if (restBombas_ < maxBombas_)
    {
        restBombas_++;
    }
}

bool Player::esInvencible() const
{
    return invencible_;
}

void Player::activarInvencibilidad()
{
    invencible_ = true;
    invencibleClock.restart();
}

void Player::actualizarInvencibilidad()
{
    // El jugador es invencible por 2 segundos despues de recibir daño
    if (invencible_ &&
        invencibleClock.getElapsedTime().asSeconds() >= 2.f)
    {
        invencible_ = false;
    }
}
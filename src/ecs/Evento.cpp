#include "ecs/Evento.hpp"


Evento::Evento(EventType tipo, int autor, int objetivo, int posicionX, int posicionY, EventData data)
{
    this -> tipo_ = tipo;
    this -> autor_ = autor;
    this -> objetivo_ = objetivo;
    this -> posicionX_ = posicionX;
    this -> posicionY_ = posicionY;
    this -> data_ = data;
}

Evento Evento::playerMove(int autorId,  int posX,  int posY, int dx, int dy)
{
    EventData data;
    data.mover = {dx, dy};

    return Evento(EventType::PlayerMove, autorId, -1, posX, posY, data);
}

Evento Evento::playerPlaceBomb(int autorId,  int posX, int posY)
{
    return Evento(EventType::PlayerPlaceBomb, autorId, -1, posX, posY, {});
}

Evento Evento::playerDeath(int autorId, int playerId,  int posX,  int posY)
{
    return Evento(EventType::PlayerDeath, autorId, playerId, posX, posY, {});
}

Evento Evento::playerPickPowerUp(int autorId, int powerUpId,  int posX,  int posY, int powerUpTipo)
{
    EventData data;
    data.powerUp = {powerUpTipo};

    return Evento(EventType::PlayerPickPowerUp, autorId, powerUpId, posX, posY, data);
}

Evento Evento::manualEvent(EventType tipo, int autor, int objetivo, int posicionX, int posicionY, EventData data)
{
    return Evento(tipo, autor, objetivo, posicionX, posicionY, data);
} 

Evento Evento::enemyMove(int enemyId, int posX, int posY, int dx, int dy)
{
    EventData data;
    data.mover = {dx, dy};

    return Evento(
        EventType::EnemyMove,
        enemyId,
        -1,
        posX,
        posY,
        data
    );
}

Evento Evento::bombExplode(int bombId, int creador, int posX, int posY, int radio)
{
    EventData data;
    data.explosion.creador = creador;
    data.explosion.radio = radio;

    return Evento(EventType::BombExplode,bombId,-1,posX,posY,data);
}

Evento Evento::tileDestroyed(int posX, int posY)
{
    return Evento(
        EventType::TileDestroyed,
        -1,
        -1,
        posX,
        posY,
        {}
    );
}

Evento Evento::enemyDeath(int enemyId,int killerId,int posX,int posY)
{
    return Evento(EventType::EnemyDeath,killerId,enemyId,posX,posY,{});
}

Evento Evento::chainExplosion(int bombId)
{
    return Evento(
        EventType::ChainExplosion,
        bombId,
        -1,
        0,
        0,
        {}
    );
}

/*
 Evento Evento::gameOver(){}
 Evento Evento::roundStart(){}
 */

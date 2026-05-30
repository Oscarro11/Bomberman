#pragma once

#include <optional>

#include "ecs/Evento.hpp"
#include "utils/Directions.hpp"

class Personaje
{
protected:
    int id_;
    int posX_;
    int posY_;
    int vida_;
    

public:
    virtual ~Personaje() = default;

    virtual Evento generarEventoMov(Directions direction) = 0;
    virtual std::optional<Evento> recibirDanio(const Personaje& atacante) = 0;

    int id() const {return id_;};
    int posX() const {return posX_;};
    int posY() const {return posY_;};
    int vida() const {return vida_;};
    bool isAlive() const {return vida_ > 0;};
    
    void setPosition(int x, int y)
    {
        posX_ = x;
        posY_ = y;
    }

};
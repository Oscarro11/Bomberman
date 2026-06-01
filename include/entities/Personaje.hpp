#pragma once
#include <optional>
#include "ecs/Evento.hpp"
#include "utils/Directions.hpp"

class Personaje
{
protected:
    int id_;
    unsigned int posX_;
    unsigned int posY_;
    unsigned int vida_;

public:
    virtual ~Personaje() = default;

    virtual Evento generarEventoMov(Directions direction) = 0;
    virtual std::optional<Evento> recibirDanio(const Personaje& atacante) = 0;

    int          id()   const { return id_; }
    unsigned int posX() const { return posX_; }
    unsigned int posY() const { return posY_; }
    unsigned int vida() const { return vida_; }
    void setPosX(unsigned int x) { posX_ = x; }
    void setPosY(unsigned int y) { posY_ = y; }
};
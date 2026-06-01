#pragma once

#include <SFML/System.hpp>

class Bomba
{
private:

    int id_;
    int creador;
    int posX;
    int posY;
    int radio;

    sf::Time duracion;
    sf::Clock reloj_;

public:

    Bomba(int id, int creador, int x, int y, int radio, sf::Time duracion);

    void generarEvento();

    int getId() const;

    int getCreador();
    
    int getRadio();

    int getX();

    int getY();

    bool expirada() const;
};
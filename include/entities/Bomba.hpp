#pragma once

#include <SFML/System.hpp>

class Bomba
{
private:

    int creador;

    int posX;

    int posY;

    sf::Time duracion;

public:

    Bomba(int creador, int x, int y, sf::Time duracion);

    void generarEvento();

    int getX();

    int getY();
};
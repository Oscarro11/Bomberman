#pragma once

#include <SFML/System.hpp>
#include "utils/GameConstants.hpp"

class Explosion
{
private:
    int id_;
    int posX;
    int posY;
    int creatorId_;

    sf::Time duracion_;
    sf::Clock reloj_;

public:
    Explosion(int id,int posX,int posY,sf::Time duracion, int creatorId);

    int getId() const;

    int getX();

    int getY();

    int getCreatorId() const ;

    bool expirada() const;
};
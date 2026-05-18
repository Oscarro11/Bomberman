#ifndef POWERUP_H
#define POWERUP_H

class PowerUp
{
private:

    int tipo;

    int posX;

    int posY;

public:

    PowerUp(int tipo, int posX, int posY);

    int getX();

    int getY();
};

#endif
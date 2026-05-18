#ifndef ENEMIGO_H
#define ENEMIGO_H

class Enemigo
{
private:

    int tipo;

    bool recibioDano;

    int alcance;

    int posX;

    int posY;

public:

    Enemigo(int tipo, int alcance, int x, int y);

    int getX();

    int getY();
};

#endif
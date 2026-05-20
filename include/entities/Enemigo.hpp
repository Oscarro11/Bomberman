#pragma once

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
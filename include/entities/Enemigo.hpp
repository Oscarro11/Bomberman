#pragma once

class Enemigo
{
private:

    int tipo_;

    bool recibioDano_;

    int alcance_;

    int posX_;

    int posY_;

public:

    Enemigo(int tipo, int alcance, int x, int y);

    int getX();

    int getY();

    void setPosition(int x, int y);
};
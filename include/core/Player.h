#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:

    int vida;

    int maxBombas;

    int rangoExplosion;

    double velocidad;

    int spawnPointX;

    int spawnPointY;

public:

    Player(
        int vida,
        int maxBombas,
        double velocidad,
        int spawnX,
        int spawnY
    );

    void actualizarStat(int tipo, int cantidad);

    void colocarBomba();

    int getX();

    int getY();
};

#endif
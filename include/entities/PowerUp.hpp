#pragma once

enum PowerUpType{
    maxBomb,
    explosionRange,
    speed
};

class PowerUp
{
    private:
        PowerUpType tipo;
        int posX;
        int posY;    

    public:
        PowerUp(PowerUpType tipo, int posX, int posY);
        ~PowerUp() = default;
};

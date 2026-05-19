#pragma once

enum PowerUpType{
    maxBomb,
    explosionRange,
    speed
};

class PowerUp
{
    private:
        PowerUpType tipo_;
        int posX_;
        int posY_;    

    public:
        PowerUp(PowerUpType tipo, int posX, int posY);
        ~PowerUp() = default;
};

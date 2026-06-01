#pragma once

enum PowerUpType : int{
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

        PowerUpType tipo() const;
        
        int posX() const;
        int posY() const;
};

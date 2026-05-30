#pragma once

struct PlayerStats{
    int maxBombas = 3;
    int rangoExplosion = 3;
    int velocidad = 3;
};

struct PlayerConfig{
    PlayerStats stats;
    bool isSelected;
};
#pragma once

struct PlayerStats{
    unsigned int maxBombas = 3;
    unsigned int rangoExplosion = 3;
    double velocidad = 3;
};

struct PlayerConfig{
    PlayerStats stats;
    bool isSelected;
};
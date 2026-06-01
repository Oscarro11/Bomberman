#pragma once

#include <string>

struct PlayerStats{
    std::string nombre = "";

    int maxBombas = 3;
    int rangoExplosion = 3;
    int velocidad = 3;
};

struct PlayerConfig{
    PlayerStats stats;
    bool isSelected;
};
#pragma once

#include <vector>

#include "utils/GameConstants.hpp"

struct PlayerData{
    int id, vida, maxBombas, bombsPlaced, rangoExplosion, velocidad;
};

struct HUDData{
    float roundTime;
    bool gameOver;
    int numPlayers;
};

struct RenderSnapshot {
    std::vector<std::vector<TileType>>      tiles;
    std::vector<std::vector<EntityType>>    entities;
    std::vector<PlayerData>                 players;
    HUDData                                 hud;
};

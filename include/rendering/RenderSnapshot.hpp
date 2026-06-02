#pragma once

#include <vector>
#include <string>

#include "utils/GameConstants.hpp"

struct PlayerData{
    std::string nombre;
    int id, vida, maxBombas, bombsPlaced, rangoExplosion, puntaje;
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

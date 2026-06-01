#pragma once
#include <vector>
#include <string>

struct PlayerSnapshot {
    int id;
    unsigned int posX;
    unsigned int posY;
    unsigned int vida;
};

struct RenderSnapshot {
    std::vector<std::string> grid;
    unsigned int gridAncho;
    unsigned int gridAlto;
    std::vector<PlayerSnapshot> jugadores;
};

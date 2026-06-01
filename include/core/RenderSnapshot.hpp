#pragma once
#include <vector>
#include <string>

struct PlayerSnapshot {
    int id;
    unsigned int posX;
    unsigned int posY;
    unsigned int vida;
};

struct BombSnapshot {
    unsigned int posX;
    unsigned int posY;
};

struct ExplosionCell {
    unsigned int posX;
    unsigned int posY;
};

struct RenderSnapshot {
    std::vector<std::string> grid;
    unsigned int gridAncho;
    unsigned int gridAlto;
    std::vector<PlayerSnapshot>  jugadores;
    std::vector<BombSnapshot>    bombas;
    std::vector<ExplosionCell>   explosiones;
    int winner; // -1=jugando, >=0=id ganador, -2=todos muertos
};

#include "core/Tablero.hpp"
#include <fstream>

Tablero::Tablero() : ancho_(0), alto_(0) {}

bool Tablero::cargar(const std::string& archivo) {
    std::ifstream file(archivo);
    if (!file.is_open()) return false;

    grid_.clear();
    std::string linea;
    while (std::getline(file, linea)) {
        if (!linea.empty())
            grid_.push_back(linea);
    }

    alto_  = grid_.size();
    ancho_ = alto_ > 0 ? grid_[0].size() : 0;
    return true;
}

char Tablero::getTile(int x, int y) const {
    if (!dentroDelMapa(x, y)) return '#';
    return grid_[y][x];
}

void Tablero::setTile(int x, int y, char tile) {
    if (dentroDelMapa(x, y))
        grid_[y][x] = tile;
}

bool Tablero::dentroDelMapa(int x, int y) const {
    return x >= 0 && y >= 0
        && static_cast<unsigned int>(x) < ancho_
        && static_cast<unsigned int>(y) < alto_;
}

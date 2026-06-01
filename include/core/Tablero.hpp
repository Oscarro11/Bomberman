#pragma once
#include <string>
#include <vector>

class Tablero {
    std::vector<std::string> grid_;
    unsigned int ancho_;
    unsigned int alto_;

public:
    Tablero();
    bool cargar(const std::string& archivo);

    char getTile(int x, int y) const;
    void setTile(int x, int y, char tile);
    bool dentroDelMapa(int x, int y) const;

    unsigned int ancho() const { return ancho_; }
    unsigned int alto()  const { return alto_; }
};

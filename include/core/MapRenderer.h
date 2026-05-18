#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <vector>
#include <string>

class MapRenderer
{
private:
    std::vector<std::string> mapa;

public:
    bool cargarMapa(const std::string& nombreArchivo);

    std::vector<std::string> obtenerMapa() const;

    int obtenerFilas() const;

    int obtenerColumnas() const;

    void mostrarMapa();
};

#endif
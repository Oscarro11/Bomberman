#include "core/MapRenderer.h"
#include "core/Tablero.h"

int main()
{
    Tablero tablero("assets/mapas/mapa.txt");

    MapRenderer renderer;

    renderer.render(tablero);

    return 0;
}
#include "MapRenderer.h"
#include <cassert>
#include <iostream>

using namespace std;

void testCargarMapaCorrectamente()
{
    MapRenderer lector;

    bool resultado = lector.cargarMapa("assets/mapas/mapa_test.txt");

    assert(resultado == true);

    cout << "testCargarMapaCorrectamente OK\n";
}

void testCantidadFilas()
{
    MapRenderer lector;

    lector.cargarMapa("assets/mapas/mapa_test.txt");

    assert(lector.obtenerFilas() == 3);

    cout << "testCantidadFilas OK\n";
}

void testCantidadColumnas()
{
    MapRenderer lector;

    lector.cargarMapa("assets/mapas/mapa_test.txt");

    assert(lector.obtenerColumnas() == 5);

    cout << "testCantidadColumnas OK\n";
}

void testContenidoMapa()
{
    MapRenderer lector;

    lector.cargarMapa("assets/mapas/mapa_test.txt");

    vector<string> mapa = lector.obtenerMapa();

    assert(mapa[0] == "#####");
    assert(mapa[1] == "#@..#");
    assert(mapa[2] == "#####");

    cout << "testContenidoMapa OK\n";
}

void testArchivoInexistente()
{
    MapRenderer lector;

    bool resultado = lector.cargarMapa("assets/mapas/no_existe.txt");

    assert(resultado == false);

    cout << "testArchivoInexistente OK\n";
}

int main()
{
    testCargarMapaCorrectamente();

    testCantidadFilas();

    testCantidadColumnas();

    testContenidoMapa();

    testArchivoInexistente();

    cout << "\nTodas las pruebas pasaron correctamente\n";

    return 0;
}
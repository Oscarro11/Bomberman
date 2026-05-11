#include "MapRenderer.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

using namespace std;

bool MapRenderer::cargarMapa(const string& nombreArchivo)
{
    mapa.clear();

    ifstream archivo(nombreArchivo);

    if (!archivo.is_open())
    {
        return false;
    }

    string linea;

    while (getline(archivo, linea))
    {
        mapa.push_back(linea);
    }

    archivo.close();

    return true;
}

vector<string> MapRenderer::obtenerMapa() const
{
    return mapa;
}

int MapRenderer::obtenerFilas() const
{
    return mapa.size();
}

int MapRenderer::obtenerColumnas() const
{
    if (mapa.empty())
    {
        return 0;
    }

    return mapa[0].size();
}

void MapRenderer::mostrarMapa()
{
    sf::Font font;

    if (!font.openFromFile("assets/fonts/consola.ttf"))
    {
        cout << "No se pudo cargar la fuente\n";
        return;
    }

    const int FONT_SIZE = 32;

    int filas = obtenerFilas();
    int columnas = obtenerColumnas();

    sf::RenderWindow window(
        sf::VideoMode(
            sf::Vector2u(columnas * FONT_SIZE,
                         filas * FONT_SIZE)
        ),
        "Bomberman"
    );

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                sf::Text texto(font);

                texto.setString(string(1, mapa[i][j]));

                texto.setCharacterSize(FONT_SIZE);

                texto.setPosition(
                    sf::Vector2f(j * FONT_SIZE,
                                 i * FONT_SIZE)
                );

                texto.setFillColor(sf::Color::White);

                window.draw(texto);
            }
        }

        window.display();
    }
}
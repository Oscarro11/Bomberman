#include "MapRenderer.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void mostrarMapa(const string& nombreArchivo)
{
    vector<string> mapa;

    ifstream archivo(nombreArchivo);

    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el archivo\n";
        return;
    }

    string linea;

    while (getline(archivo, linea))
    {
        mapa.push_back(linea);
    }

    archivo.close();

    sf::Font font;

    if (!font.openFromFile("assets/fonts/consola.ttf"))
    {
        cout << "No se pudo cargar la fuente\n";
        return;
    }

    const int FONT_SIZE = 32;

    int filas = mapa.size();
    int columnas = mapa[0].size();

    sf::RenderWindow window(
        sf::VideoMode(
            sf::Vector2u(columnas * FONT_SIZE,
                         filas * FONT_SIZE)
        ),
        "Bomberman Console"
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
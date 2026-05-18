#include "core/MapRenderer.h"
#include "core/Tablero.h"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

void MapRenderer::render(Tablero& tablero)
{
    sf::Font font;

    if (!font.loadFromFile("assets/fonts/consola.ttf"))
    {
        cout << "No se pudo cargar la fuente\n";
        return;
    }

    const int FONT_SIZE = 32;

    int filas = tablero.getRows();
    int columnas = tablero.getColumns();

    sf::RenderWindow window(
        sf::VideoMode(
            columnas * FONT_SIZE,
            filas * FONT_SIZE
        ),
        "Bomberman"
    );

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                int cell = tablero.getCell(i, j);

                char simbolo = '.';

                if (cell == 11)
                    simbolo = '@';

                else if (cell == 12)
                    simbolo = '$';

                else if (cell / 10 == 2)
                    simbolo = '1';

                else if (cell / 10 == 3)
                    simbolo = '!';

                else if (cell == 4)
                    simbolo = '0';

                else if (cell == 5)
                    simbolo = '#';

                else if (cell == 6)
                    simbolo = '+';

                else if (cell == 7)
                    simbolo = '/';

                else if (cell == 8)
                    simbolo = '.';

                else if (cell / 10 == 9)
                    simbolo = '?';

                sf::Text texto;

                texto.setFont(font);

                texto.setString(string(1, simbolo));

                texto.setCharacterSize(FONT_SIZE);

                texto.setPosition(
                    sf::Vector2f(
                        j * FONT_SIZE,
                        i * FONT_SIZE
                    )
                );

                texto.setFillColor(sf::Color::White);

                window.draw(texto);
            }
        }

        window.display();
    }
}
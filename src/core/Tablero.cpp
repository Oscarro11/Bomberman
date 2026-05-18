#include "Tablero.h"

#include <fstream>
#include <iostream>

using namespace std;

Tablero::Tablero(string source)
{
    ifstream archivo(source);

    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el mapa\n";
        return;
    }

    string linea;

    while (getline(archivo, linea))
    {
        vector<int> fila;

        for (int i = 0; i < linea.size(); i++)
        {
            char c = linea[i];

            switch(c)
            {
                case '@':
                {
                    listaPlayers.push_back(
                        Player(3, 1, 1.0, tablero.size(), i)
                    );

                    fila.push_back(10 + listaPlayers.size());

                    break;
                }

                case '$':
                {
                    listaPlayers.push_back(
                        Player(3, 1, 1.0, tablero.size(), i)
                    );

                    fila.push_back(10 + listaPlayers.size());

                    break;
                }

                case '1':
                {
                    listaEnemigos.push_back(
                        Enemigo(1, 5, tablero.size(), i)
                    );

                    fila.push_back(20 + listaEnemigos.size());

                    break;
                }

                case '!':
                {
                    listaBombas.push_back(
                        Bomba(
                            0,
                            tablero.size(),
                            i,
                            sf::seconds(3)
                        )
                    );

                    fila.push_back(30 + listaBombas.size());

                    break;
                }

                case '0':
                    fila.push_back(4);
                    break;

                case '#':
                    fila.push_back(5);
                    break;

                case '+':
                    fila.push_back(6);
                    break;

                case '/':
                    fila.push_back(7);
                    break;

                case '.':
                    fila.push_back(8);
                    break;

                case '?':
                {
                    listaPowerUps.push_back(
                        PowerUp(1, tablero.size(), i)
                    );

                    fila.push_back(90 + listaPowerUps.size());

                    break;
                }

                default:
                    fila.push_back(8);
                    break;
            }
        }

        tablero.push_back(fila);
    }

    archivo.close();
}

int Tablero::getCell(int row, int column)
{
    return tablero[row][column];
}

void Tablero::setCell(int id, int row, int column)
{
    tablero[row][column] = id;
}

int Tablero::getRows()
{
    return tablero.size();
}

int Tablero::getColumns()
{
    if (tablero.empty())
    {
        return 0;
    }

    return tablero[0].size();
}

vector<vector<int>> Tablero::getBoard()
{
    return tablero;
}

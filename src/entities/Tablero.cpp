#include "entities/Tablero.hpp"

#include <fstream>
#include <iostream>

Tablero::Tablero(string source)
{
    //This should be changed, to take into account source
    //ifstream archivo(source);
    ifstream archivo("/home/oscar/bomberman/assets/mapas/mapa.txt");

    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el mapa\n";
        return;
    }

    //pthread_mutex_init(&boardAccessMutex, NULL);

    string linea;
    int y = 0;

    while (getline(archivo, linea))
    {
        vector<CellContent> fila;

        for (int i = 0; i < linea.size(); i++)
        {
            char c = linea[i];

            switch(c)
            {
                case '@':
                {
                    listaSpawnPlayers[0] = CellSpawn{Cell{i, y}, 11};
                    fila.push_back(CellContent{Player1, 11});
                    break;
                }

                case '$':
                {
                    listaSpawnPlayers[1] = CellSpawn{Cell{i, y}, 12};
                    fila.push_back(CellContent{Player2, 12});
                    break;
                }

                case '1':
                {
                    listaSpawnEnemies.push_back(CellSpawn{Cell{i, y}, 21 + (int) listaSpawnEnemies.size()});
                    fila.push_back(CellContent{Enemy, 20 + listaSpawnEnemies.size()});
                    break;
                }

                //This case shouldnt exist, as no bombs are preloaded
                case '!':
                {
                    fila.push_back(CellContent{Bomb, 30});
                    break;
                }

                //This case shouldnt exist, as no explosions are preloaded
                case '0':
                    fila.push_back(CellContent{Explosion, 40});
                    break;

                case '#':
                    fila.push_back(CellContent{Wall, nullopt});
                    break;

                case '+':
                    fila.push_back(CellContent{DestructibleWall, nullopt});
                    break;

                case '/':
                    fila.push_back(CellContent{Hole, nullopt});
                    break;

                case '.':
                    fila.push_back(CellContent{Floor, nullopt});
                    break;

                case '?':
                {
                    listaSpawnPowerUps.push_back(CellSpawn{Cell{i, y}, 81 + (int) listaSpawnPowerUps.size()});
                    fila.push_back(CellContent{PowerUpItem, 80 + listaSpawnPowerUps.size()});

                    break;
                }

                default:
                    fila.push_back(CellContent{Floor, nullopt});
                    break;
            }
        }

        tablero.push_back(fila);
    }

    archivo.close();
}

Tablero::~Tablero()
{
    //pthread_mutex_destroy(&boardAccessMutex);
}

CellContent Tablero::getCell(Cell cell)
{
    //pthread_mutex_lock(&boardAccessMutex);
    return tablero[cell.y][cell.x];
    ////pthread_mutex_unlock(&boardAccessMutex);
}

void Tablero::setCell(BoardElement type, int id, Cell cell)
{
    //pthread_mutex_lock(&boardAccessMutex);
    tablero[cell.y][cell.x] = CellContent{type, id};
    //pthread_mutex_unlock(&boardAccessMutex);
}

std::array<CellSpawn, 4> Tablero::getPlayersSpawn()
{
    return listaSpawnPlayers;
};

std::vector<CellSpawn> Tablero::getEnemiesSpawn()
{
    return listaSpawnEnemies;
}

std::vector<CellSpawn> Tablero::getPowerUpsSpawn()
{
    return listaSpawnPowerUps;
}


int Tablero::getWidth()
{
    //pthread_mutex_lock(&boardAccessMutex);
    return tablero.size();
    //pthread_mutex_unlock(&boardAccessMutex);
}

int Tablero::getHeight()
{
    //pthread_mutex_lock(&boardAccessMutex);
    if (tablero.empty())
    {
        return 0;
    }

    return tablero[0].size();
    //pthread_mutex_unlock(&boardAccessMutex);
}

vector<vector<CellContent>> Tablero::getBoard()
{
    //pthread_mutex_lock(&boardAccessMutex);
    return tablero;
    //pthread_mutex_unlock(&boardAccessMutex);
}

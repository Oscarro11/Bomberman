#pragma once

#include <vector>
#include <string>
#include <array>
#include <optional>
#include <pthread.h>

#include "entities/Player.hpp"
#include "entities/Bomba.hpp"
#include "entities/Enemigo.hpp"
#include "entities/PowerUp.hpp"

using namespace std;

enum BoardElement {
    Player1,
    Player2,
    Player3,
    Player4,
    Enemy,
    Bomb,
    Explosion,
    Wall,
    DestructibleWall,
    Hole,
    Floor,
    PowerUpItem
};

struct CellContent{
    BoardElement type;
    std::optional<int> id;
};

struct Cell{
    int x, y;
};

struct CellSpawn{
    Cell spawnPoint;
    int id;
};

class Tablero
{
    private:
        //pthread_mutex_t boardAccessMutex;
        vector<vector<CellContent>> tablero;

        vector<Player> listaPlayers;
        vector<Enemigo> listaEnemigos;
        vector<Bomba> listaBombas;
        vector<PowerUp> listaPowerUps;

        std::array<CellSpawn, 4> listaSpawnPlayers;
        vector<CellSpawn> listaSpawnEnemies;
        vector<CellSpawn> listaSpawnPowerUps;

    public:
        Tablero(string source);
        virtual ~Tablero();

        CellContent getCell(Cell cell);

        void setCell(BoardElement type, int id, Cell cell);

        std::array<CellSpawn, 4> getPlayersSpawn();
        std::vector<CellSpawn> getEnemiesSpawn();
        std::vector<CellSpawn> getPowerUpsSpawn();

        //std::vector<Cell> setEnemies(std::vector<Enemigo*> enemies);

        int getWidth();

        int getHeight();

        vector<vector<CellContent>> getBoard();
};

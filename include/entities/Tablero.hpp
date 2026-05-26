#pragma once

#include <vector>
#include <string>
#include <array>
#include <optional>
#include <pthread.h>

//TODO: reorganize enums and constructs to divide terrain and entities
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
        std::vector<std::vector<CellContent>> tablero;

        std::array<CellSpawn, 4> listaSpawnPlayers;
        std::vector<CellSpawn> listaSpawnEnemies;
        std::vector<CellSpawn> listaSpawnPowerUps;

        /*TODO: implement vector that has all cells with certain type 
        std::array<CellSpawn, 4> listaCellPlayers;
        std::vector<CellSpawn> listaEnemigos;
        std::vector<CellSpawn> listaBombas;
        std::vector<CellSpawn> listaPowerUps;
        */

    public:
        Tablero(std::string source);
        virtual ~Tablero();

        CellContent getCell(Cell cell);

        void setCell(BoardElement type, int id, Cell cell);

        std::array<CellSpawn, 4> getPlayersSpawn();
        std::vector<CellSpawn> getEnemiesSpawn();
        std::vector<CellSpawn> getPowerUpsSpawn();

        //std::vector<Cell> setEnemies(std::vector<Enemigo*> enemies);

        int getWidth();

        int getHeight();

        std::vector<std::vector<CellContent>> getBoard();
};

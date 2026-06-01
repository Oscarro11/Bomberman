#include "entities/Tablero.hpp"
#include "utils/GameConstants.hpp"
#include "utils/AssetsUtils.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

Tablero::Tablero(const std::string& source)
{
    pthread_mutex_init(&board_mutex_, NULL);

    loadMap(source);
}

Tablero::~Tablero()
{
    pthread_mutex_destroy(&board_mutex_);
}

void Tablero::loadMap(const std::string& source)
{
    std::filesystem::path path = AssetPaths::MAPS / source;
    std::ifstream file(path);

    if (!file)
    {
        throw std::runtime_error(
            "Could not open map file: " + source
        );
    }

    std::string line;
    int y = 0;

    pthread_mutex_lock(&board_mutex_);
    while (std::getline(file, line))
    {
        std::vector<BoardCell> row;

        for (int x = 0; x < line.size(); ++x)
        {
            BoardCell cell;
            char c = line[x];

            switch (c)
            {
                case '#': cell.terrainType = TileType::Wall;        break;
                case '+': cell.terrainType = TileType::Breakable;   break;
                case '/': cell.terrainType = TileType::Hole;        break;
                case '.': cell.terrainType = TileType::Floor;       break;

                case '@':
                    cell.terrainType = TileType::Floor;
                    listaSpawnPlayers_[0] = SpawnPoint{Position{x, y}, 1};
                    break;

                case '$':
                    cell.terrainType = TileType::Floor;
                    listaSpawnPlayers_[1] = SpawnPoint{Position{x, y}, 2};
                    break;

                case '1':
                    cell.terrainType = TileType::Floor;
                    listaSpawnEnemies_.push_back(Position{x, y});
                    break;

                case '?':
                    cell.terrainType = TileType::Floor;
                    listaSpawnPowerUps_.push_back(Position{x, y});
                    break;
            }

            row.push_back(cell);
        }

        matrix_.push_back(row);

        ++y;
    }
    pthread_mutex_unlock(&board_mutex_);
}

bool Tablero::isWalkable(Position p) const {

    pthread_mutex_lock(&board_mutex_);
    if (matrix_[p.y][p.x].terrainType != TileType::Floor){
        pthread_mutex_unlock(&board_mutex_);
        return false;
    }
    
    std::vector<Occupant> occupants = matrix_[p.y][p.x].occupants;
    pthread_mutex_unlock(&board_mutex_);

    for (Occupant entity : occupants)
    {
        if (entity.type == EntityType::Player ||
            entity.type == EntityType::Enemy ||
            entity.type == EntityType::Bomb)
            return false;
    }
    
    return true;
}

const BoardCell Tablero::getCell(Position p) const
{
    pthread_mutex_lock(&board_mutex_);
    BoardCell copy = matrix_[p.y][p.x];
    pthread_mutex_unlock(&board_mutex_);

    return copy;
}

//This method resets the tile in the position parameter
void Tablero::setTerrain(Position p, TileType terrain)
{
    pthread_mutex_lock(&board_mutex_);
    matrix_[p.y][p.x] = BoardCell{terrain, std::vector<Occupant>()};
    pthread_mutex_unlock(&board_mutex_);
}

void Tablero::addOccupant(Position p, const Occupant &occ)
{
    pthread_mutex_lock(&board_mutex_);
    matrix_[p.y][p.x].occupants.push_back(occ);
    pthread_mutex_unlock(&board_mutex_);
}

bool Tablero::removeOccupant(Position pos,EntityType type, int id)
{
    pthread_mutex_lock(&board_mutex_);
    auto& occupants = matrix_[pos.y][pos.x].occupants;
    pthread_mutex_unlock(&board_mutex_);

    auto it = std::remove_if(
    occupants.begin(),
    occupants.end(),
    [type,id](const Occupant& occ)
    {
        return occ.type == type &&
               occ.entityId == id;
    }
    );

    pthread_mutex_lock(&board_mutex_);
    occupants.erase(it, occupants.end());
    pthread_mutex_unlock(&board_mutex_);

    bool removed = (it != occupants.end());
    return removed;
}

bool Tablero::moveOccupant(Position from, Position to, int id)
{
    pthread_mutex_lock(&board_mutex_);
    auto& fromOcc = matrix_[from.y][from.x].occupants;
    pthread_mutex_unlock(&board_mutex_);

    auto it = std::find_if(
        fromOcc.begin(),
        fromOcc.end(),
        [id](const Occupant& occ)
        { return occ.entityId == id;});

    if (it == fromOcc.end())
    {
        return false;
    }

    Occupant occ = *it;

    pthread_mutex_lock(&board_mutex_);
    fromOcc.erase(it);
    matrix_[to.y][to.x].occupants.push_back(occ);
    pthread_mutex_unlock(&board_mutex_);

    return true;
}

const std::vector<std::vector<BoardCell>> Tablero::matrix() const
{
    pthread_mutex_lock(&board_mutex_);
    auto copy = matrix_;
    pthread_mutex_unlock(&board_mutex_);

    return copy;
}
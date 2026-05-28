#include "entities/Tablero.hpp"
#include "utils/GameConstants.hpp"
#include "utils/AssetsUtils.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

Tablero::Tablero(const std::string& source)
{
    loadMap(source);
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

    unsigned int y = 0;

    while (std::getline(file, line))
    {
        std::vector<BoardCell> row;

        for (unsigned int x = 0; x < line.size(); ++x)
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

                case '?':
                    cell.terrainType = TileType::Floor;
                    listaSpawnPowerUps_.push_back(Position{x, y});
            }

            row.push_back(cell);
        }

        matrix_.push_back(row);

        ++y;
    }
}

bool Tablero::isWalkable(Position p) const {
    if (matrix_[p.y][p.x].terrainType != TileType::Floor)
        return false;
       
    for (Occupant entity : matrix_[p.y][p.x].occupants)
    {
        if (entity.type == EntityType::Player ||
            entity.type == EntityType::Enemy ||
            entity.type == EntityType::Bomb)
            return false;
    }
    
    return true;
}

const BoardCell &Tablero::getCell(Position p) const
{
    return matrix_[p.y][p.x];
}

//This method resets the tile in the position parameter
void Tablero::setTerrain(Position p, TileType terrain)
{
    matrix_[p.y][p.x] = BoardCell{terrain, std::vector<Occupant>()};
}

void Tablero::addOccupant(Position p, const Occupant &occ)
{
    matrix_[p.y][p.x].occupants.push_back(occ);
}

bool Tablero::removeOccupant(Position pos, int id)
{
    auto& occupants = matrix_[pos.y][pos.x].occupants;
    auto it = std::remove_if(
        occupants.begin(),
        occupants.end(),
        [id](const Occupant& occ)
        { return occ.entityId == id;});

    bool removed = (it != occupants.end());
    occupants.erase(it, occupants.end());

    return removed;
}

bool Tablero::moveOccupant(Position from, Position to, int id)
{
    auto& fromOcc = matrix_[from.y][from.x].occupants;
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
    fromOcc.erase(it);

    matrix_[to.y][to.x].occupants.push_back(occ);
    return true;
}

const std::vector<std::vector<BoardCell>>& Tablero::matrix() const
{
    return matrix_;
}
#pragma once

#include <vector>
#include <string>
#include <array>
#include <pthread.h>

#include "utils/GameConstants.hpp"
#include "utils/Position.hpp"

struct Occupant{
    EntityType type;
    int entityId;

    bool operator==(const Occupant& other) const {
        return type == other.type && entityId == other.entityId;
    }
};

struct SpawnPoint{
    Position position;
    int entityId;

    bool operator==(const SpawnPoint other) const {
        return position == other.position && entityId == other.entityId;
    }
};

struct BoardCell{
    TileType terrainType;
    std::vector<Occupant> occupants;

    bool operator==(const BoardCell& other) const {
        return terrainType == other.terrainType && occupants == other.occupants;
    }
};

class Tablero {
    public:
        explicit Tablero(const std::string& source);
        void loadMap(const std::string& source);

        ~Tablero() = default;
        bool isWalkable(Position p) const;

        //For testing purposes
        const std::vector<std::vector<BoardCell>>& matrix() const;

        const BoardCell& getCell(Position p) const;
        void setTerrain(Position p, TileType terrain);

        void addOccupant(Position p, const Occupant& occ);
        bool removeOccupant(Position p, int id);
        bool moveOccupant(Position from, Position to, int id);

        int getHeight() const {return matrix_.size();};
        int getWidth() const {return matrix_[0].size();};

        std::array<SpawnPoint, 4> getSpawnPlayers() {return listaSpawnPlayers_;};
        std::vector<Position> getSpawnEnemies() {return listaSpawnEnemies_;};
        std::vector<Position> getSpawnPowerUps() {return listaSpawnPowerUps_;};

    private:
        std::vector<std::vector<BoardCell>> matrix_;

        std::array<SpawnPoint, 4> listaSpawnPlayers_;
        std::vector<Position> listaSpawnEnemies_;
        std::vector<Position> listaSpawnPowerUps_;

        //pthread_mutex_t mutex_;
};

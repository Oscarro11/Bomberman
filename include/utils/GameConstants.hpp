#pragma once

const int MAX_PLAYERS = 4;
const int MIN_PLAYERS = 2;

enum class TileType{
    Floor,
    Hole,
    Wall,
    Breakable
};

enum class EntityType{
    Player,
    Player1,
    Player2,
    Player3,
    Player4,
    Enemy,
    Bomb,
    Explosion,
    PowerUpItem,
    None
};

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};
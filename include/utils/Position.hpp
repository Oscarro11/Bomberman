#pragma once
struct Position{
    unsigned int x, y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};
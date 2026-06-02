#include <cassert>
#include <iostream>
#include <sstream>

#include "core/Engine.hpp"
#include "rendering/RenderSnapshot.hpp"
#include "ecs/PlayerStats.hpp"

std::string boardToString(
    const RenderSnapshot& snapshot)
{
    std::stringstream ss;

    const auto& tiles =
        snapshot.tiles;

    const auto& entities =
        snapshot.entities;

    for (size_t y = 0;
         y < tiles.size();
         ++y)
    {
        for (size_t x = 0;
             x < tiles[y].size();
             ++x)
        {
            EntityType entity =
                entities[y][x];

            // -------------------------
            // Entities have priority
            // -------------------------

            switch (entity)
            {
                case EntityType::Player1:
                    ss << '@';
                    continue;

                case EntityType::Player2:
                    ss << '$';
                    continue;

                case EntityType::Player3:
                    ss << '&';
                    continue;

                case EntityType::Player4:
                    ss << '%';
                    continue;

                case EntityType::Enemy:
                    ss << 'E';
                    continue;

                case EntityType::Bomb:
                    ss << 'B';
                    continue;

                case EntityType::Explosion:
                    ss << 'X';
                    continue;

                case EntityType::PowerUpItem:
                    ss << 'P';
                    continue;

                case EntityType::None:
                    break;

                default:
                    ss << '?';
                    continue;
            }

            // -------------------------
            // Terrain rendering
            // -------------------------

            switch (tiles[y][x])
            {
                case TileType::Wall:
                    ss << '#';
                    break;

                case TileType::Breakable:
                    ss << '+';
                    break;

                case TileType::Floor:
                    ss << '.';
                    break;

                case TileType::Hole:
                    ss << 'O';
                    break;

                default:
                    ss << '?';
                    break;
            }
        }

        ss << '\n';
    }

    return ss.str();
}

void runTest()
{
    std::vector<PlayerStats> playerStats;

    playerStats.push_back(PlayerStats{"", 3, 3, 0});
    playerStats.push_back(PlayerStats{"", 3, 3, 0});

    Engine engine(
        "mapa_test.txt",
        playerStats
    );

    // -------------------------
    // Expected snapshot
    // -------------------------

    RenderSnapshot expected;

    expected.tiles =
    {
        {
            TileType::Wall,
            TileType::Wall,
            TileType::Wall,
            TileType::Wall,
            TileType::Wall
        },

        {
            TileType::Wall,
            TileType::Floor,
            TileType::Floor,
            TileType::Floor,
            TileType::Wall
        },

        {
            TileType::Wall,
            TileType::Floor,
            TileType::Floor,
            TileType::Floor,
            TileType::Wall
        },

        {
            TileType::Wall,
            TileType::Wall,
            TileType::Wall,
            TileType::Wall,
            TileType::Wall
        }
    };

    expected.entities =
    {
        {
            EntityType::None,
            EntityType::None,
            EntityType::None,
            EntityType::None,
            EntityType::None
        },

        {
            EntityType::None,
            EntityType::Player1,
            EntityType::None,
            EntityType::None,
            EntityType::None
        },

        {
            EntityType::None,
            EntityType::None,
            EntityType::None,
            EntityType::Player2,
            EntityType::None
        },

        {
            EntityType::None,
            EntityType::None,
            EntityType::None,
            EntityType::None,
            EntityType::None
        }
    };

    // -------------------------
    // Actual snapshot
    // -------------------------

    RenderSnapshot actual =
        engine.makeRenderSnapshot();

    // -------------------------
    // Compare boards visually
    // -------------------------

    std::string expectedBoard =
        boardToString(expected);

    std::string actualBoard =
        boardToString(actual);

    if (expectedBoard != actualBoard)
    {
        std::cout
            << "EXPECTED:\n"
            << expectedBoard
            << '\n';

        std::cout
            << "ACTUAL:\n"
            << actualBoard
            << '\n';

        assert(false);
    }
}

int main()
{
    runTest();

    std::cout
        << "RenderSnapshot test passed\n";

    return 0;
}
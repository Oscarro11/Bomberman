#include <cassert>
#include "entities/Tablero.hpp"
#include "utils/GameConstants.hpp"

void compareBoards(
    const std::vector<std::vector<BoardCell>>& expected,
    const std::vector<std::vector<BoardCell>>& actual)
{
    assert(expected.size() == actual.size());

    for (size_t y = 0; y < expected.size(); ++y)
    {
        assert(expected[y].size() == actual[y].size());

        for (size_t x = 0;
             x < expected[y].size();
             ++x)
        {
            const BoardCell& e =
                expected[y][x];

            const BoardCell& a =
                actual[y][x];

            // Terrain mismatch
            if (e.terrainType != a.terrainType)
            {
                printf(
                    "Terrain mismatch at (%zu, %zu)\n",
                    x,
                    y
                );

                printf(
                    "Expected terrain: %d\n",
                    (int)e.terrainType
                );

                printf(
                    "Actual terrain: %d\n",
                    (int)a.terrainType
                );

                assert(false);
            }

            // Occupant count mismatch
            if (e.occupants.size()
                != a.occupants.size())
            {
                printf(
                    "Occupant count mismatch "
                    "at (%zu, %zu)\n",
                    x,
                    y
                );

                printf(
                    "Expected: %zu\n",
                    e.occupants.size()
                );

                printf(
                    "Actual: %zu\n",
                    a.occupants.size()
                );

                assert(false);
            }

            // Occupant mismatch
            for (size_t i = 0;
                 i < e.occupants.size();
                 ++i)
            {
                const Occupant& eo =
                    e.occupants[i];

                const Occupant& ao =
                    a.occupants[i];

                if (!(eo == ao))
                {
                    printf(
                        "Occupant mismatch "
                        "at (%zu, %zu)\n",
                        x,
                        y
                    );

                    printf(
                        "Expected entity: %d\n",
                        (int)eo.type
                    );

                    printf(
                        "Actual entity: %d\n",
                        (int)ao.type
                    );

                    printf(
                        "Expected id: %d\n",
                        eo.entityId
                    );

                    printf(
                        "Actual id: %d\n",
                        ao.entityId
                    );

                    assert(false);
                }
            }
        }
    }
}

void runTest(){
    Tablero tablero = Tablero("mapa_test.txt");
    std::vector<std::vector<BoardCell>> produced = tablero.matrix();

    std::vector<std::vector<BoardCell>> expected =
    {
        {
            { TileType::Wall, {} },
            { TileType::Wall, {} },
            { TileType::Wall, {} },
            { TileType::Wall, {} },
            { TileType::Wall, {} }
        },

        {
            { TileType::Wall, {} },
            { TileType::Floor, {} },
            { TileType::Floor, {} },
            { TileType::Floor, {} },
            { TileType::Wall, {} }
        },

        {
            { TileType::Wall, {} },
            { TileType::Wall, {} },
            { TileType::Wall, {} },
            { TileType::Wall, {} },
            { TileType::Wall, {} }
        }
    };

    compareBoards(expected, produced);
}

int main() {
    runTest();
    return 0;
}
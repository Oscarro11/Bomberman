#include "rendering/GameScreen.hpp"
#include "rendering/MainMenuScreen.hpp"
#include "utils/ScreenUtils.hpp"
#include "utils/GameConstants.hpp"

using namespace ScreenUtils;

Screen* GameScreen::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape)
        return new MainMenuScreen();
    return Screen::STAY;
}

void GameScreen::update(const RenderSnapshot& snapshot) {
    snapshot_ = snapshot;   // plain copy — render thread reads this
}

void GameScreen::render(sf::RenderWindow& window,
                         const sf::Font& font) const {
    int boardHeight = snapshot_.tiles.size();

    // 1. HUD at row 0
    renderHUD(window, font, 0);

    // 2. Board starts at row 2 (one blank line after HUD)
    int boardStartRow = 2;
    renderBoard(window, font, boardStartRow);

    // 3. Player cards below the board
    int cardsStartRow = boardStartRow + boardHeight + 1;
    renderPlayerCards(window, font, cardsStartRow);
}

// ─────────────────────────────────────────────────
// HUD — above the board
// ─────────────────────────────────────────────────

void GameScreen::renderHUD(sf::RenderWindow& window,
                            const sf::Font& font,
                            int row) const {
    drawSegmentsAt(window, font, {
        { " round: ",                                          COL_DIM    },
        { std::to_string((int)snapshot_.hud.roundTime) + "s", COL_ACCENT },
        { "   players: ",                                      COL_DIM    },
        { std::to_string(snapshot_.hud.numPlayers),            COL_VALUE  },
        { snapshot_.hud.gameOver ? "   GAME OVER" : "",
          sf::Color(237, 9, 9)                                            },
    }, row, 0);
}

// ─────────────────────────────────────────────────
// Board
// ─────────────────────────────────────────────────

void GameScreen::renderBoard(sf::RenderWindow& window,
                              const sf::Font& font,
                              int startRow) const {
    for (int row = 0; row < (int)snapshot_.tiles.size(); ++row) {
        std::vector<TextSegment> segmentos;

        for (int col = 0; col < (int)snapshot_.tiles[row].size(); ++col) {
            TileType   terrain = snapshot_.tiles[row][col];
            EntityType entity  = snapshot_.entities[row][col];

            if (entity != EntityType::None) {
                switch (entity) {
                    case EntityType::Player1:
                        segmentos.push_back({"@ ", sf::Color(252,   3, 211)}); break;
                    case EntityType::Player2:
                        segmentos.push_back({"$ ", sf::Color( 17,   5, 245)}); break;
                    case EntityType::Player3:
                        segmentos.push_back({"& ", sf::Color(237,   9,   9)}); break;
                    case EntityType::Player4:
                        segmentos.push_back({"% ", sf::Color(245, 225,   2)}); break;
                    case EntityType::Enemy:
                        segmentos.push_back({"E ", sf::Color(237,   9,   9)}); break;
                    case EntityType::Bomb:
                        segmentos.push_back({"! ", sf::Color(245, 225,   2)}); break;
                    case EntityType::Explosion:
                        segmentos.push_back({"* ", sf::Color(245, 164,   2)}); break;
                    case EntityType::PowerUpItem:
                        segmentos.push_back({"? ", sf::Color( 11, 222, 208)}); break;
                    default:
                        segmentos.push_back({"  ", COL_DEFAULT});             break;
                }
            } else {
                switch (terrain) {
                    case TileType::Wall:
                        segmentos.push_back({"# ", sf::Color(130, 120, 120)}); break;
                    case TileType::Breakable:
                        segmentos.push_back({"+ ", sf::Color( 94,  44,  30)}); break;
                    case TileType::Hole:
                        segmentos.push_back({"/ ", sf::Color( 40,  40,  40)}); break;
                    case TileType::Floor:
                        segmentos.push_back({". ", sf::Color( 44, 232,   7)}); break;
                    default:
                        segmentos.push_back({"  ", COL_DEFAULT});              break;
                }
            }
        }

        // startRow offsets the board down past the HUD
        drawSegmentsAt(window, font, segmentos, startRow + row, 0);
    }
}

// ─────────────────────────────────────────────────
// Player cards — all side by side below the board
// ─────────────────────────────────────────────────

void GameScreen::renderPlayerCards(sf::RenderWindow& window,
                                    const sf::Font& font,
                                    int startRow) const {
    const sf::Color PLAYER_COLORS[] = {
        sf::Color(252,   3, 211),   // P1 magenta
        sf::Color( 17,   5, 245),   // P2 blue
        sf::Color(237,   9,   9),   // P3 red
        sf::Color(245, 225,   2),   // P4 yellow
    };

    // Each card occupies CARD_COLS columns
    // Must match the width of the strings drawn inside
    const int CARD_COLS = 22;

    for (int i = 0; i < (int)snapshot_.players.size(); ++i) {
        const PlayerData& p   = snapshot_.players[i];
        sf::Color         col = PLAYER_COLORS[i % 4];
        int               startCol = i * CARD_COLS;

        // Row 0 — top border
        drawLineAt(window, font,
            "+--------------------+",
            startRow + 0, startCol, COL_BORDER);

        // Row 1 — player name
        drawSegmentsAt(window, font, {
            { "| ",                          COL_BORDER },
            { p.nombre.length() > 0 ? p.nombre : "P" + std::to_string(i + 1)
              + std::string(18, ' '),         col        },
            { "|",                            COL_BORDER },
        }, startRow + 1, startCol);

        // Row 2 — vida
        drawSegmentsAt(window, font, {
            { "| Vida:    ",   COL_DIM   },
            { std::to_string(p.vida),
              p.vida > 1 ? COL_VALUE
                         : sf::Color(237, 9, 9) },
            { "          |",  COL_BORDER },
        }, startRow + 2, startCol);

        // Row 3 — bombas restantes / max
        drawSegmentsAt(window, font, {
            { "| Bombas:  ",  COL_DIM   },
            { std::to_string(p.maxBombas - p.bombsPlaced)
              + "/" + std::to_string(p.maxBombas), COL_VALUE },
            { "       |",    COL_BORDER },
        }, startRow + 3, startCol);

        // Row 4 — rango
        drawSegmentsAt(window, font, {
            { "| Rango:   ",  COL_DIM   },
            { std::to_string(p.rangoExplosion), COL_VALUE },
            { "          |",  COL_BORDER },
        }, startRow + 4, startCol);

        // Row 5 — puntaje
        drawSegmentsAt(window, font, {
            { "| Score:     ",  COL_DIM   },
            { std::to_string(p.puntaje), COL_VALUE },
            { "        |",  COL_BORDER },
        }, startRow + 5, startCol);

        // Row 6 — bottom border
        drawLineAt(window, font,
            "+--------------------+",
            startRow + 6, startCol, COL_BORDER);
    }
}
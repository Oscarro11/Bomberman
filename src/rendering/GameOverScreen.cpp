#include "rendering/MultiplayerGameOverScreen.hpp"
#include "rendering/MainMenuScreen.hpp"

using namespace ScreenUtils;

const sf::Color GameOverScreen::PLAYER_COLORS[4] = {
    sf::Color(252,   3, 211),   // P1 magenta
    sf::Color( 17,   5, 245),   // P2 blue
    sf::Color(237,   9,   9),   // P3 red
    sf::Color(245, 225,   2),   // P4 yellow
};

GameOverScreen::GameOverScreen(const std::string& winnerName, int winnerId)
    : winnerName_(winnerName)
    , winnerId_(winnerId)
{}

Screen* GameOverScreen::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
        selectedOption_ = (selectedOption_ + 1) % 2;
    if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
        selectedOption_ = (selectedOption_ + 1) % 2;

    if (key == sf::Keyboard::Return) {
        if (selectedOption_ == 0) return new MainMenuScreen();
        if (selectedOption_ == 1) return new ExitScreen();
    }

    return Screen::STAY;
}

void GameOverScreen::render(sf::RenderWindow& window,
                             const sf::Font& font) const {
    sf::Color winnerColor = PLAYER_COLORS[winnerId_ % 4];
    int row = 2;

    // Header
    drawLine(window, font,
        " +-----------------------------------------------+", row++, COL_BORDER);
    drawLine(window, font,
        " |               GAME  OVER                     |", row++, COL_ACCENT);
    drawLine(window, font,
        " +-----------------------------------------------+", row++, COL_BORDER);
    row++;

    // Winner ASCII art
    drawLine(window, font,
        "          .---------.",                             row++, winnerColor);
    drawLine(window, font,
        "         /  *     *  \\",                           row++, winnerColor);
    drawLine(window, font,
        "        |    WINNER!  |",                           row++, winnerColor);
    drawLine(window, font,
        "         \\  *     *  /",                           row++, winnerColor);
    drawLine(window, font,
        "          `---------'",                             row++, winnerColor);
    row++;

    // Winner name
    drawLine(window, font,
        "               \\o/",                               row++, winnerColor);
    drawLine(window, font,
        "               /|\\",                               row++, winnerColor);
    drawLine(window, font,
        "               / \\",                               row++, winnerColor);
    row++;

    drawSegmentsAt(window, font, {
        { "          Gano: ",   COL_DIM       },
        { "P" + std::to_string(winnerId_ + 1)
          + " " + winnerName_, winnerColor   },
    }, row++, 0);
    row++;

    // Divider
    drawLine(window, font,
        " +-----------------------------------------------+", row++, COL_BORDER);
    row++;

    // Options
    drawSegments(window, font, {
        { "    ",                                            COL_DEFAULT  },
        { selectedOption_ == 0 ? "> [ MENU PRINCIPAL ] <"
                               : "  [ MENU PRINCIPAL ]  ", selectedOption_ == 0
                                                          ? COL_SELECTED
                                                          : COL_DEFAULT  },
        { "       ",                                         COL_DEFAULT  },
        { selectedOption_ == 1 ? "> [ SALIR ] <"
                               : "  [ SALIR ]  ",           selectedOption_ == 1
                                                          ? COL_SELECTED
                                                          : COL_DEFAULT  },
    }, row++);
    row++;

    drawLine(window, font,
        "  [A/D] navegar   [ENTER] confirmar",               row, COL_DIM);
}
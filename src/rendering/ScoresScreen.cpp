#include "rendering/ScoresScreen.hpp"
#include "rendering/MainMenuScreen.hpp"
#include "utils/ScreenUtils.hpp"

#include <algorithm>

using namespace ScreenUtils;

ScoresScreen::ScoresScreen()
{
    scores_ = ScoreManager::load();

    // Sort by score descending on construction
    std::sort(scores_.begin(), scores_.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });
}

Screen* ScoresScreen::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape || key == sf::Keyboard::Return)
        return new MainMenuScreen();
    return Screen::STAY;
}

void ScoresScreen::render(sf::RenderWindow& window,
                           const sf::Font& font) const {
    int row = 0;

    // Header
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    drawLine(window, font, " |              PUNTAJES                         |", row++, COL_ACCENT);
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    row++;

    // Column headers
    drawSegmentsAt(window, font, {
        { "  #  ", COL_DIM     },
        { "NOMBRE         ", COL_ACCENT  },
        { "SCORE  ", COL_ACCENT  },
        { "WINS  ", COL_ACCENT  },
        { "DEATHS", COL_ACCENT  },
    }, row++, 0);
    drawLine(window, font, "  -----  -------------  -------  ------  ------", row++, COL_BORDER);

    // Score rows
    if (scores_.empty()) {
        row++;
        drawLine(window, font, "  no hay puntajes registrados todavia.", row++, COL_DIM);
    } else {
        for (int i = 0; i < (int)scores_.size(); ++i) {
            const ScoreEntry& e = scores_[i];

            // Rank color — gold, silver, bronze, rest
            sf::Color rankColor = i == 0 ? sf::Color(232, 232,  48)   // gold
                                : i == 1 ? sf::Color(192, 192, 192)   // silver
                                : i == 2 ? sf::Color(205, 127,  50)   // bronze
                                :          COL_DIM;

            // Pad name to fixed width
            std::string paddedName = e.name;
            if ((int)paddedName.size() < 13)
                paddedName += std::string(13 - paddedName.size(), ' ');
            else
                paddedName = paddedName.substr(0, 13);

            drawSegmentsAt(window, font, {
                { "  " + std::to_string(i + 1) + ".  ",  rankColor   },
                { paddedName + "  ",                      COL_DEFAULT },
                { std::to_string(e.score) + "      ",     COL_VALUE   },
                { std::to_string(e.wins)  + "       ",    COL_DEFAULT },
                { std::to_string(e.deaths),               COL_DEFAULT },
            }, row++, 0);
        }
    }

    row++;
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    drawLine(window, font, "  [ESC] / [ENTER]  volver al menu",            row,   COL_DIM);
}
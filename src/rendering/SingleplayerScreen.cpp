#include "rendering/SingleplayerScreen.hpp"
#include "rendering/MainMenuScreen.hpp"
#include "rendering/Screen.hpp"
#include "utils/ScreenUtils.hpp"

using namespace ScreenUtils;

SinglePlayerConfigurationScreen::SinglePlayerConfigurationScreen() {}

Difficulty SinglePlayerConfigurationScreen::getDifficulty() const {
    return difficulty_;
}

char SinglePlayerConfigurationScreen::keyToChar(sf::Keyboard::Key key) const {
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
        return 'A' + (key - sf::Keyboard::A);
    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
        return '0' + (key - sf::Keyboard::Num0);
    return 0;
}

Screen* SinglePlayerConfigurationScreen::handleInput(sf::Keyboard::Key key) {

    // ── Typing mode ──────────────────────────────────
    if (typingName_) {
        if (key == sf::Keyboard::Return || key == sf::Keyboard::Tab) {
            typingName_     = false;
            selectedOption_ = 2;   // move to start after confirming name
            return Screen::STAY;
        }
        if (key == sf::Keyboard::BackSpace) {
            if (!playerName_.empty())
                playerName_.pop_back();
            return Screen::STAY;
        }
        if (key == sf::Keyboard::Escape) {
            typingName_ = false;
            return Screen::STAY;
        }
        if ((int)playerName_.size() < MAX_NAME_LEN) {
            char c = keyToChar(key);
            if (c != 0) playerName_ += c;
        }
        return Screen::STAY;
    }

    // ── Normal navigation ────────────────────────────
    if (key == sf::Keyboard::W)
        selectedOption_ = (selectedOption_ + 3) % 4;

    if (key == sf::Keyboard::S)
        selectedOption_ = (selectedOption_ + 1) % 4;

    if (key == sf::Keyboard::A && selectedOption_ == 0) {
        if (difficulty_ == Difficulty::Medium) difficulty_ = Difficulty::Easy;
        else if (difficulty_ == Difficulty::Hard)  difficulty_ = Difficulty::Medium;
    }

    if (key == sf::Keyboard::D && selectedOption_ == 0) {
        if (difficulty_ == Difficulty::Easy)   difficulty_ = Difficulty::Medium;
        else if (difficulty_ == Difficulty::Medium) difficulty_ = Difficulty::Hard;
    }

    if (key == sf::Keyboard::Return) {
        if (selectedOption_ == 1) {   // name field selected — enter typing mode
            typingName_ = true;
            return Screen::STAY;
        }
        if (selectedOption_ == 2) return new StartScreen();
        if (selectedOption_ == 3) return new MainMenuScreen();
    }

    if (key == sf::Keyboard::Escape)
        return new MainMenuScreen();

    return Screen::STAY;
}

void SinglePlayerConfigurationScreen::render(
    sf::RenderWindow& window,
    const sf::Font& font
) const {
    int row = 0;

    drawLine(window, font,
        " +-----------------------------------------------+", row++, COL_BORDER);
    drawLine(window, font,
        " |           SINGLE PLAYER                      |", row++, COL_ACCENT);
    drawLine(window, font,
        " +-----------------------------------------------+", row++, COL_BORDER);
    row++;

    // Difficulty selector — option 0
    std::string diffText = "Difficulty:  ";
    switch (difficulty_) {
        case Difficulty::Easy:   diffText += "< EASY   >"; break;
        case Difficulty::Medium: diffText += "< MEDIUM >"; break;
        case Difficulty::Hard:   diffText += "< HARD   >"; break;
    }
    drawLine(window, font,
        selectedOption_ == 0 ? "> " + diffText + " <"
                             : "  " + diffText,
        row++,
        selectedOption_ == 0 ? COL_SELECTED : COL_DEFAULT);
    row++;

    // Name field — option 1
    bool nameActive = (selectedOption_ == 1);

    std::string nameDisplay = playerName_;
    if (typingName_) nameDisplay += "_";
    nameDisplay.resize(MAX_NAME_LEN, ' ');

    drawSegments(window, font, {
        { nameActive ? "> " : "  ",   COL_SELECTED                          },
        { "Name: ",                    nameActive ? COL_SELECTED : COL_DIM   },
        { nameDisplay,                 typingName_ ? COL_VALUE
                                     : nameActive  ? COL_DEFAULT
                                     :               COL_DIM                 },
        { nameActive ? " <" : "  ",   COL_SELECTED                          },
        { nameActive
            ? "  [ENTER] escribir"
            : "",                      COL_DIM                               },
    }, row++);
    row++;

    // Start — option 2
    drawLine(window, font,
        selectedOption_ == 2 ? "> [ INICIAR ] <"
                             : "  [ INICIAR ]  ",
        row++,
        selectedOption_ == 2 ? COL_SELECTED : COL_DEFAULT);

    // Back — option 3
    drawLine(window, font,
        selectedOption_ == 3 ? "> [ VOLVER ]  <"
                             : "  [ VOLVER ]   ",
        row++,
        selectedOption_ == 3 ? COL_SELECTED : COL_DEFAULT);
    row++;

    drawLine(window, font, "  [W/S] navegar   [A/D] dificultad   [ENTER] confirmar", row, COL_DIM);
}
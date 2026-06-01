#include "rendering/SingleplayerScreen.hpp"
#include "rendering/MainMenuScreen.hpp"
#include "rendering/Screen.hpp"
#include "utils/ScreenUtils.hpp"

using namespace ScreenUtils;

SinglePlayerConfigurationScreen::
SinglePlayerConfigurationScreen()
{
}

Difficulty
SinglePlayerConfigurationScreen::getDifficulty() const
{
    return difficulty_;
}

Screen*
SinglePlayerConfigurationScreen::handleInput(
    sf::Keyboard::Key key
)
{
    if (key == sf::Keyboard::W)
    {
        selectedOption_ =
            (selectedOption_ + 2) % 3;
    }

    if (key == sf::Keyboard::S)
    {
        selectedOption_ =
            (selectedOption_ + 1) % 3;
    }

    if (key == sf::Keyboard::A)
    {
        if (selectedOption_ == 0)
        {
            if (difficulty_ == Difficulty::Medium)
                difficulty_ = Difficulty::Easy;

            else if (difficulty_ == Difficulty::Hard)
                difficulty_ = Difficulty::Medium;
        }
    }

    if (key == sf::Keyboard::D)
    {
        if (selectedOption_ == 0)
        {
            if (difficulty_ == Difficulty::Easy)
                difficulty_ = Difficulty::Medium;

            else if (difficulty_ == Difficulty::Medium)
                difficulty_ = Difficulty::Hard;
        }
    }

    if (key == sf::Keyboard::Return)
    {
        // START
        if (selectedOption_ == 1)
        {
            return new StartScreen;
        }

        // BACK
        if (selectedOption_ == 2)
        {
            return new MainMenuScreen;
        }
    }

    if (key == sf::Keyboard::Escape)
    {
        return new MainMenuScreen;
        
    }

    return Screen::STAY;
}

void SinglePlayerConfigurationScreen::render(
    sf::RenderWindow& window,
    const sf::Font& font
) const
{
    drawLine(
        window,
        font,
        " +---------------------------------------------------------------+",
        0,
        COL_BORDER
    );

    drawLine(
        window,
        font,
        "*** SINGLE PLAYER ***",
        3,
        COL_ACCENT
    );

    std::string diffText =
        "Difficulty: ";

    switch (difficulty_)
    {
        case Difficulty::Easy:
            diffText += "EASY";
            break;

        case Difficulty::Medium:
            diffText += "MEDIUM";
            break;

        case Difficulty::Hard:
            diffText += "HARD";
            break;
    }

    drawLine(
        window,
        font,
        selectedOption_ == 0
            ? "> " + diffText + " <"
            : "  " + diffText,
        6,
        selectedOption_ == 0
            ? COL_SELECTED
            : COL_DEFAULT
    );

    drawLine(
        window,
        font,
        selectedOption_ == 1
            ? "> [ Iniciar ] <"
            : "  [ Iniciar ]",
        8,
        selectedOption_ == 1
            ? COL_SELECTED
            : COL_DEFAULT
    );

    drawLine(
        window,
        font,
        selectedOption_ == 2
            ? "> [ Volver ] <"
            : "  [ Volver ]",
        9,
        selectedOption_ == 2
            ? COL_SELECTED
            : COL_DEFAULT
    );

    drawLine(
        window,
        font,
        "[W/S] Navigate",
        12,
        COL_DEFAULT
    );

    drawLine(
        window,
        font,
        "[A/D] Change Difficulty",
        13,
        COL_DEFAULT
    );

    drawLine(
        window,
        font,
        "[ENTER] Continue",
        14,
        COL_DEFAULT
    );

    drawLine(
        window,
        font,
        "[Escape] Back to menu",
        15,
        COL_DEFAULT
    );
}
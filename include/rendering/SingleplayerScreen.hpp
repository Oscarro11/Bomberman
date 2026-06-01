#pragma once

#include "rendering/Screen.hpp"
#include "utils/GameConstants.hpp"



class SinglePlayerConfigurationScreen : public Screen
{
public:
    SinglePlayerConfigurationScreen();

    Screen* handleInput(sf::Keyboard::Key key) override;
    void render(
        sf::RenderWindow& window,
        const sf::Font& font
    ) const override;

    Difficulty getDifficulty() const;

private:
    int selectedOption_ = 0;

    Difficulty difficulty_ =
        Difficulty::Easy;
};
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
    std::string getPlayerName() const { return playerName_; }

private:
    int selectedOption_ = 0;

    Difficulty difficulty_ = Difficulty::Easy;

    bool        typingName_     = false;
    std::string playerName_     = "";

    static constexpr int MAX_NAME_LEN = 10;

    char keyToChar(sf::Keyboard::Key key) const;
};
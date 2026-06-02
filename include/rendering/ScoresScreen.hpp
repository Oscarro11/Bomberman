#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "systems/ScoreManager.hpp"
#include "rendering/Screen.hpp"

class ScoresScreen : public Screen {
    public:
        ScoresScreen();

        Screen* handleInput(sf::Keyboard::Key key) override;
        void    render(sf::RenderWindow& window,
                    const sf::Font& font) const override;

    private:
        std::vector<ScoreEntry> scores_;
};
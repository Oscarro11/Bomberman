#pragma once

#include "rendering/Screen.hpp"
#include "rendering/MainMenuScreen.hpp"

class InstructionsScreen : public Screen {
    public:
        Screen* handleInput(sf::Keyboard::Key key) override;
        void render(sf::RenderWindow& window, const sf::Font& font) const override;
};
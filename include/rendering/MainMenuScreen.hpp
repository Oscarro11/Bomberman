#pragma once

#include "rendering/Screen.hpp"

class MainMenuScreen : public Screen {
    public:
        Screen* handleInput(sf::Keyboard::Key key) override;
        void render(sf::RenderWindow& window, const sf::Font& font) const override;

    private:
        int selectedOption_ = 0;   // what number of menu is currently selected
};
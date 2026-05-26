#pragma once

//TODO: review includes
#include "rendering/Screen.hpp"
#include "entities/Tablero.hpp"
#include "utils/ScreenUtils.hpp"

class GameScreen : public Screen {
    public:
        GameScreen();
    
        Screen* handleInput(sf::Keyboard::Key key) override;
        void render(sf::RenderWindow& window, const sf::Font& font) const override;

    private:

        //TODO: this class shouldnt know the board directly, instead using Snapshots from the Engine to render
        Tablero* tablero_;
};
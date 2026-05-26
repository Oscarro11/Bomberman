#pragma once

#include <vector>
#include <SFML/Graphics.hpp> 

#include "utils/ScreenUtils.hpp"

//Abstract class to manage screen rendering for various menus
class Screen {
    public:
        virtual ~Screen() = default;

        // Returns the next screen to transition to, or nullptr to stay
        virtual Screen* handleInput(sf::Keyboard::Key key) = 0;
        virtual void render(sf::RenderWindow& window, const sf::Font& font) const = 0;

        inline static Screen* STAY = nullptr;       
};

class ExitScreen : public Screen {
    public: 
        Screen* handleInput(sf::Keyboard::Key key) override {return Screen::STAY;};
        void render(sf::RenderWindow& window, const sf::Font& font) const override {};
};

class StartScreen : public Screen {
    public: 
        Screen* handleInput(sf::Keyboard::Key key) override {return Screen::STAY;};
        void render(sf::RenderWindow& window, const sf::Font& font) const override {};
};
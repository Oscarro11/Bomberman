#include "rendering/Vista.hpp"
#include "rendering/MainMenuScreen.hpp"
#include "utils/ScreenUtils.hpp"
#include <sstream>

Vista::Vista(sf::RenderWindow& window, sf::Font& font)
    : window_(window)
    , font_(font)
    , currentScreen_(new MainMenuScreen)
{
    ScreenUtils::setCharSize(window);
}

Vista::~Vista() {
    delete currentScreen_;
}

void Vista::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    Screen* next = currentScreen_ -> handleInput(event.key.code);

    if (next != nullptr) {
        /*
        // Check if NewGameScreen signaled start
        if (dynamic_cast<NewMultiplayerMenuScreen*>(currentScreen_) &&
            dynamic_cast<MainMenuScreen*>(next)) {
            // back to main — not starting
        }
        if (dynamic_cast<NewMultiplayerMenuScreen*>(next) == nullptr &&
            dynamic_cast<SettingsScreen*>(next) == nullptr) {
            startGame_ = true;   // no known screen → start game
            return;
        }
        */

        if (dynamic_cast<EmptyScreen*>(next))
        {
            window_.close();
            return;
        }
        
       
        delete currentScreen_;
        currentScreen_ = next;
    }
}

void Vista::render() {
    if (window_.isOpen()){

        window_.clear(sf::Color::Black);
        currentScreen_ -> render(window_, font_);
        window_.display();
    }
}
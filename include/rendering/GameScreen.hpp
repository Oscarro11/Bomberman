#pragma once

#include <SFML/Graphics.hpp>

#include "rendering/Screen.hpp"
#include "rendering/RenderSnapshot.hpp"

class GameScreen : public Screen {
    public:
        Screen* handleInput(sf::Keyboard::Key key) override;

        // New signature — takes snapshot instead of accessing Tablero directly
        void render(sf::RenderWindow& window,
                    const sf::Font& font) const;
            
        // Called before render() each frame to feed the snapshot
        void update(const RenderSnapshot& snapshot);

    private:
        RenderSnapshot snapshot_;   // stored copy, render reads from this

        void renderHUD (sf::RenderWindow&, const sf::Font&, int row) const;
        void renderBoard (sf::RenderWindow&, const sf::Font&, int startRow) const;
        void renderPlayerCards(sf::RenderWindow&, const sf::Font&, int startRow) const;
};
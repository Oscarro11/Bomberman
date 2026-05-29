#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "utils/GameConstants.hpp"
#include "ecs/PlayerStats.hpp"

class Screen;

class Vista {
    public:
        Vista(sf::RenderWindow& window, sf::Font& font);
        ~Vista();

        void handleEvent(const sf::Event& event);
        void render();
        bool shouldStartGame() const { return startGame_; }
        std::vector<PlayerStats*> getPlayerStats() const;

    private:
        sf::RenderWindow& window_;
        sf::Font&         font_;
        Screen*           currentScreen_;   // owns the active screen
        bool              startGame_ = false;
};
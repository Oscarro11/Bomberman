#pragma once

#include <SFML/Graphics.hpp>
#include <utils/GameConstants.hpp>
#include <rendering/Screen.hpp>
#include <array>

class Vista {
    public:
        Vista(sf::RenderWindow& window, sf::Font& font);
        ~Vista();

        void handleEvent(const sf::Event& event);
        void render();
        bool shouldStartGame() const { return startGame_; }
        //std::vector<PlayerConfig> getPlayerConfigs() const;

    private:
        sf::RenderWindow& window_;
        sf::Font&         font_;
        Screen*           currentScreen_;   // owns the active screen
        bool              startGame_ = false;
};
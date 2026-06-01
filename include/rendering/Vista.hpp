#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "utils/GameConstants.hpp"
#include "ecs/PlayerStats.hpp"
#include "rendering/RenderSnapshot.hpp"

class Screen;

class Vista {
    public:
        Vista(sf::RenderWindow& window, sf::Font& font);
        ~Vista();

        void handleEvent(const sf::Event& event);
        void render();
        bool shouldStartGame() const { return startGame_; }
        std::vector<PlayerStats> getPlayerStats() const;

        void updateSnapshot(RenderSnapshot& snap);
        void transitionToGame();
        Difficulty getDifficulty() const;
        
        
        private:
        std::vector<PlayerStats> cachedStats_;
        Difficulty cachedDifficulty_ =
        Difficulty::Easy;
        
        sf::RenderWindow& window_;
        sf::Font&         font_;
        Screen*           currentScreen_;   // owns the active screen
        bool              startGame_ = false;
};
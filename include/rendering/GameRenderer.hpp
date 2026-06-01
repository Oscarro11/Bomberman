#pragma once
#include <SFML/Graphics.hpp>
#include "core/RenderSnapshot.hpp"

class GameRenderer {
    sf::Font& font_;
    float tileSize_;

public:
    GameRenderer(sf::Font& font, float tileSize = 48.f);
    void draw(sf::RenderWindow& window, const RenderSnapshot& snapshot);

private:
    void drawBoard(sf::RenderWindow& window, const RenderSnapshot& snapshot);
    void drawPlayers(sf::RenderWindow& window, const RenderSnapshot& snapshot);
    void drawHUD(sf::RenderWindow& window, const RenderSnapshot& snapshot);
};

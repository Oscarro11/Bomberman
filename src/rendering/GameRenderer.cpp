#include "rendering/GameRenderer.hpp"
#include <string>

static const sf::Color COLOR_WALL      (80,  80,  80);
static const sf::Color COLOR_BREAKABLE (160, 100, 40);
static const sf::Color COLOR_EMPTY     (30,  30,  30);
static const sf::Color PLAYER_COLORS[4] = {
    sf::Color(0,   150, 255),
    sf::Color(255, 80,  80),
    sf::Color(80,  220, 80),
    sf::Color(255, 200, 0)
};

GameRenderer::GameRenderer(sf::Font& font, float tileSize)
    : font_(font), tileSize_(tileSize) {}

void GameRenderer::draw(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    window.clear(sf::Color::Black);
    drawBoard(window, snapshot);
    drawPlayers(window, snapshot);
    drawHUD(window, snapshot);
    window.display();
}

void GameRenderer::drawBoard(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    sf::RectangleShape tile(sf::Vector2f(tileSize_ - 2.f, tileSize_ - 2.f));

    for (unsigned int y = 0; y < snapshot.gridAlto; y++) {
        for (unsigned int x = 0; x < snapshot.gridAncho && x < snapshot.grid[y].size(); x++) {
            char c = snapshot.grid[y][x];
            if      (c == '#') tile.setFillColor(COLOR_WALL);
            else if (c == '+') tile.setFillColor(COLOR_BREAKABLE);
            else               tile.setFillColor(COLOR_EMPTY);

            tile.setPosition(x * tileSize_ + 1.f, y * tileSize_ + 1.f);
            window.draw(tile);
        }
    }
}

void GameRenderer::drawPlayers(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    float radius = tileSize_ / 2.f - 4.f;
    sf::CircleShape circle(radius);

    for (const auto& p : snapshot.jugadores) {
        if (p.vida == 0) continue;
        circle.setFillColor(PLAYER_COLORS[p.id % 4]);
        circle.setPosition(
            p.posX * tileSize_ + 4.f,
            p.posY * tileSize_ + 4.f
        );
        window.draw(circle);
    }
}

void GameRenderer::drawHUD(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    float hudX = snapshot.gridAncho * tileSize_ + 10.f;
    float hudY = 10.f;

    sf::Text label;
    label.setFont(font_);
    label.setCharacterSize(14);
    label.setString("--- HUD ---");
    label.setFillColor(sf::Color::White);
    label.setPosition(hudX, hudY);
    window.draw(label);
    hudY += 25.f;

    for (const auto& p : snapshot.jugadores) {
        sf::Text text;
        text.setFont(font_);
        text.setCharacterSize(14);
        text.setFillColor(PLAYER_COLORS[p.id % 4]);

        std::string info = "P" + std::to_string(p.id + 1)
                         + "  HP: " + std::to_string(p.vida);
        if (p.vida == 0) info += "  [MUERTO]";

        text.setString(info);
        text.setPosition(hudX, hudY);
        window.draw(text);
        hudY += 24.f;
    }
}

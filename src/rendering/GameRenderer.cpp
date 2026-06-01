#include "rendering/GameRenderer.hpp"
#include <string>

static const sf::Color COLOR_WALL      (80,  80,  80);
static const sf::Color COLOR_BREAKABLE (160, 100, 40);
static const sf::Color COLOR_EMPTY     (30,  30,  30);
static const sf::Color COLOR_EXPLOSION (255, 140, 0);
static const sf::Color COLOR_BOMB      (40,  40,  40);

static const sf::Color PLAYER_COLORS[4] = {
    sf::Color(0,   150, 255),   // P1 azul
    sf::Color(255, 80,  80),    // P2 rojo
    sf::Color(80,  220, 80),    // P3 verde
    sf::Color(255, 200, 0)      // P4 amarillo
};

static const std::string PLAYER_SYMBOLS[4] = {"@", "$", "3", "4"};

static const std::string CONTROLS[4][2] = {
    {"W/A/S/D", "Q"},
    {"Flechas", "Space"},
    {"I/J/K/L", "O"},
    {"Num8/4/5/6", "Num7"}
};

GameRenderer::GameRenderer(sf::Font& font, float tileSize)
    : font_(font), tileSize_(tileSize) {}

void GameRenderer::draw(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    window.clear(sf::Color::Black);
    drawBoard(window, snapshot);
    drawExplosions(window, snapshot);
    drawBombs(window, snapshot);
    drawPlayers(window, snapshot);
    drawHUD(window, snapshot);
    if (snapshot.winner != -1)
        drawGameOver(window, snapshot);
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

void GameRenderer::drawExplosions(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    sf::RectangleShape tile(sf::Vector2f(tileSize_ - 2.f, tileSize_ - 2.f));
    tile.setFillColor(COLOR_EXPLOSION);

    sf::Text label;
    label.setFont(font_);
    label.setCharacterSize(22);
    label.setFillColor(sf::Color::White);
    label.setString("0");

    for (const auto& e : snapshot.explosiones) {
        tile.setPosition(e.posX * tileSize_ + 1.f, e.posY * tileSize_ + 1.f);
        window.draw(tile);
        label.setPosition(e.posX * tileSize_ + 14.f, e.posY * tileSize_ + 10.f);
        window.draw(label);
    }
}

void GameRenderer::drawBombs(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    float radius = tileSize_ / 2.f - 6.f;
    sf::CircleShape circle(radius);
    circle.setFillColor(COLOR_BOMB);
    circle.setOutlineColor(sf::Color(200, 200, 0));
    circle.setOutlineThickness(2.f);

    sf::Text label;
    label.setFont(font_);
    label.setCharacterSize(18);
    label.setFillColor(sf::Color(255, 220, 0));
    label.setString("!");

    for (const auto& b : snapshot.bombas) {
        circle.setPosition(b.posX * tileSize_ + 6.f, b.posY * tileSize_ + 6.f);
        window.draw(circle);
        label.setPosition(b.posX * tileSize_ + 16.f, b.posY * tileSize_ + 10.f);
        window.draw(label);
    }
}

void GameRenderer::drawPlayers(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    float radius = tileSize_ / 2.f - 4.f;
    sf::CircleShape circle(radius);

    sf::Text label;
    label.setFont(font_);
    label.setCharacterSize(16);

    for (const auto& p : snapshot.jugadores) {
        if (p.vida == 0) continue;
        sf::Color color = PLAYER_COLORS[p.id % 4];
        circle.setFillColor(color);
        circle.setPosition(p.posX * tileSize_ + 4.f, p.posY * tileSize_ + 4.f);
        window.draw(circle);

        label.setFillColor(sf::Color::Black);
        label.setString(PLAYER_SYMBOLS[p.id % 4]);
        label.setPosition(p.posX * tileSize_ + 15.f, p.posY * tileSize_ + 12.f);
        window.draw(label);
    }
}

void GameRenderer::drawHUD(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    float hudX = snapshot.gridAncho * tileSize_ + 12.f;
    float y    = 10.f;

    auto texto = [&](const std::string& str, sf::Color color, unsigned int size = 14) {
        sf::Text t;
        t.setFont(font_);
        t.setCharacterSize(size);
        t.setFillColor(color);
        t.setString(str);
        t.setPosition(hudX, y);
        window.draw(t);
        y += size + 6.f;
    };

    texto("--- HUD ---", sf::Color::White, 15);
    y += 6.f;

    for (const auto& p : snapshot.jugadores) {
        sf::Color color = PLAYER_COLORS[p.id % 4];
        std::string estado = p.vida > 0 ? "HP: " + std::to_string(p.vida) : "[MUERTO]";
        texto("P" + std::to_string(p.id + 1) + "  " + estado, color, 14);
        texto("  Mov: " + CONTROLS[p.id % 4][0], sf::Color(150,150,150), 12);
        texto("  Bomba: " + CONTROLS[p.id % 4][1], sf::Color(150,150,150), 12);
        y += 6.f;
    }

    y += 10.f;
    texto("--- TECLAS ---", sf::Color::White, 13);
    texto("ESC = salir", sf::Color(150,150,150), 12);
}

void GameRenderer::drawGameOver(sf::RenderWindow& window, const RenderSnapshot& snapshot) {
    sf::Vector2u size = window.getSize();

    sf::RectangleShape overlay(sf::Vector2f((float)size.x, (float)size.y));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text titulo;
    titulo.setFont(font_);
    titulo.setCharacterSize(42);

    std::string msg;
    sf::Color color = sf::Color::White;
    if (snapshot.winner >= 0) {
        msg   = "P" + std::to_string(snapshot.winner + 1) + " GANO!";
        color = PLAYER_COLORS[snapshot.winner % 4];
    } else {
        msg   = "EMPATE";
        color = sf::Color(200, 200, 200);
    }

    titulo.setFillColor(color);
    titulo.setString(msg);
    titulo.setPosition(size.x / 2.f - 80.f, size.y / 2.f - 60.f);
    window.draw(titulo);

    sf::Text sub;
    sub.setFont(font_);
    sub.setCharacterSize(18);
    sub.setFillColor(sf::Color::White);
    sub.setString("[ESC] para salir");
    sub.setPosition(size.x / 2.f - 80.f, size.y / 2.f + 10.f);
    window.draw(sub);
}

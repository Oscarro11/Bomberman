#include "systems/Vista.hpp"
#include "core/Engine.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bomberman");
    sf::Font font;
    font.loadFromFile("assets/fonts/consola.ttf");

    Vista vista(window, font);

    // Menu loop — runs until player hits Start
    while (window.isOpen() && !vista.shouldStartGame()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            vista.handleEvent(event);
        }
        vista.render();
    }

    /*
    // Hand off to game loop
    if (vista.shouldStartGame()) {
        Engine engine("map.txt", vista.getPlayers());
        // game loop here...
    }*/
}
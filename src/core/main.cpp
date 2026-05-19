#include "systems/Vista.hpp"
#include "core/Engine.hpp"
#include "input/InputHandler.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bomberman");
    sf::Font font;
    font.loadFromFile("assets/fonts/consola.ttf");

    Vista vista(window, font);
    sf::Clock clock;
    
    // Menu loop — runs until player hits Start
    while (window.isOpen() && !vista.shouldStartGame()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            vista.handleEvent(event);
        }
        vista.render();
    }

    // Hand off to game loop
    if (vista.shouldStartGame()) {
        std::vector<PlayerInfo> playerInfo;
        
        for (PlayerConfig config: vista.get_players_info())
        {
            playerInfo.push_back(PlayerInfo{config.vida, config.maxBombas, config.rangoExplosion, config.velocidad});
        }

        Engine engine("map.txt", playerInfo);
        InputHandler inputHandler(&engine);
        vista.clear();

        while (window.isOpen() && engine.running())
        {
            sf::Event event;
            std::vector<sf::Keyboard::Key> pressedKeys;
            sf::Time dt = clock.restart();

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed){
                    pressedKeys.push_back(event.key.code);
                }
            }
            
            if (pressedKeys.size() > 0)
            {
                inputHandler.update(pressedKeys);
            }
            
        }
    }

    return 0;
}
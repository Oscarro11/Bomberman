#include "rendering/Vista.hpp"
#include "core/Engine.hpp"
#include "input/InputHandler.hpp"
#include "utils/ScreenUtils.hpp"
#include "ecs/PlayerStats.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Bomberman", sf::Style::Fullscreen);
    sf::Font font;
    font.loadFromFile("assets/fonts/consola.ttf");
    
    ScreenUtils::setCharSize(window);
    ScreenUtils::measureFont(font);

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
       std::vector<PlayerStats> playerInfo = vista.getPlayerStats();

        Difficulty difficulty = vista.getDifficulty();

        std::string mapa = "mapa2.txt";

        if (playerInfo.size() == 1)
        {
            switch (difficulty)
            {
                case Difficulty::Easy:
                    mapa = "mapa_sp_uno.txt";
                    break;

                case Difficulty::Medium:
                    mapa = "mapa_sp_dos.txt";
                    break;

                case Difficulty::Hard:
                    mapa = "mapa_sp_tres.txt";
                    break;
            }
        }
        
        Engine engine(mapa, playerInfo);
        engine.start();

        InputHandler inputHandler(&engine);

        vista.transitionToGame();

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
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                        break;
                    }

                    pressedKeys.push_back(event.key.code);
                }
            }
            
            if (!pressedKeys.empty())
            {
                inputHandler.update(pressedKeys);
            }

            engine.update(dt);
            
            // Take snapshot under mutex
            RenderSnapshot snapshot;
            snapshot = engine.makeRenderSnapshot();

            vista.updateSnapshot(snapshot);

            // Render through base class — no arguments
            vista.render();

            sf::sleep(sf::milliseconds(16));
        }

    }
    return 0;
}
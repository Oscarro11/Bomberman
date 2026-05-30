#include "rendering/Vista.hpp"
#include "core/Engine.hpp"
#include "input/InputHandler.hpp"
#include "utils/ScreenUtils.hpp"
#include "ecs/PlayerStats.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Bomberman",
        sf::State::Fullscreen
    );

    sf::Font font("assets/fonts/consola.ttf");

    ScreenUtils::setCharSize(window);
    ScreenUtils::measureFont(font);

    Vista vista(window, font);
    sf::Clock clock;

    // Menu loop — runs until player hits Start
    while (window.isOpen() && !vista.shouldStartGame())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            vista.handleEvent(*event);
        }

        vista.render();
    }

    // Hand off to game loop
    if (vista.shouldStartGame())
    {
        std::vector<PlayerStats*> playerInfo =
            vista.getPlayerStats();

        Engine engine("mapa.txt", playerInfo);
        InputHandler inputHandler(&engine);
        
        while (window.isOpen() && engine.running())
        {

            std::vector<sf::Keyboard::Key> pressedKeys;

            clock.restart();

            while (auto event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
                else if (const auto* keyPressed =
                    event->getIf<sf::Event::KeyPressed>())
                {
                    pressedKeys.push_back(keyPressed->code);
                }
            }

            if (!pressedKeys.empty())
            {
                inputHandler.update(pressedKeys);
            }

            sf::sleep(sf::milliseconds(16));
        }
    }

    return 0;
}
#include "rendering/Vista.hpp"
#include "core/Engine.hpp"
#include "input/InputHandler.hpp"
#include "utils/ScreenUtils.hpp"
#include "ecs/PlayerStats.hpp"

enum class AppState
{
    Menu,
    Playing,
    GameOver
};

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Bomberman", sf::Style::Fullscreen);
    sf::Font font;
    font.loadFromFile("assets/fonts/consola.ttf");
    
    ScreenUtils::setCharSize(window);
    ScreenUtils::measureFont(font);

    Vista vista(window, font);
    sf::Clock clock;
    
    AppState appState = AppState::Menu;

    Engine* engine = nullptr;
    InputHandler* inputHandler = nullptr;

    // Menu loop — runs until player hits Start
    while (window.isOpen())
    {
        sf::Event event;
        std::vector<sf::Keyboard::Key> pressedKeys;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                pressedKeys.push_back(event.key.code);
            }

            vista.handleEvent(event);
        }

        switch (appState)
        {
            case AppState::Menu:
                {
                    if (vista.shouldStartGame()){
                        std::vector<PlayerStats> playerInfo =
                        vista.getPlayerStats();

                        Difficulty difficulty =
                            vista.getDifficulty();

                        std::string mapa = "mapa2.txt";

                        if (playerInfo.size() == 1)
                        {
                            switch (difficulty)
                            {
                                case Difficulty::Easy:
                                    mapa = "mapa_test_enemigos.txt";
                                    break;

                                case Difficulty::Medium:
                                    mapa = "mapa_sp_dos.txt";
                                    break;

                                case Difficulty::Hard:
                                    mapa = "mapa_sp_tres.txt";
                                    break;
                            }
                        }

                        engine = new Engine(mapa, playerInfo);
                        engine->start();

                        inputHandler =
                            new InputHandler(engine);

                        vista.transitionToGame();

                        vista.consumeStartGame();

                        appState = AppState::Playing;
                    }

                vista.render();
                break;
            }

            case AppState::Playing:
            {
                sf::Time dt = clock.restart();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                    break;
                }

                if (engine)
                {
                    inputHandler->update(pressedKeys);

                    engine->update(dt);

                    auto snapshot =
                        engine->makeRenderSnapshot();

                    vista.updateSnapshot(snapshot);

                    if (engine->isFinished())
                    {
                        vista.transitionToGameOver(
                            engine->getWinnerName(),
                            engine->getWinnerId()
                        );

                        delete inputHandler;
                        inputHandler = nullptr;

                        delete engine;
                        engine = nullptr;

                        appState = AppState::GameOver;
                    }
                }

                vista.render();
                break;
            }
            
            case AppState::GameOver:
            {
                vista.render();

                if (vista.isOnMainMenu())
                {
                    appState = AppState::Menu;
                }

                break;
            }
            sf::sleep(sf::milliseconds(16));
        }
    }

    delete inputHandler;
    delete engine;
    return 0;
}
#include "rendering/Vista.hpp"
#include "rendering/MainMenuScreen.hpp"
#include "rendering/MultiplayerScreen.hpp"
#include "rendering/GameScreen.hpp"
#include "utils/ScreenUtils.hpp"
#include <sstream>

Vista::Vista(sf::RenderWindow& window, sf::Font& font)
    : window_(window)
    , font_(font)
    , currentScreen_(new MainMenuScreen)
{
}

Vista::~Vista() {
    delete currentScreen_;
}

void Vista::handleEvent(const sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    Screen* next =
        currentScreen_->handleInput(
            event.key.code
        );

    // No transition
    if (next == nullptr ||
        next == currentScreen_)
    {
        return;
    }

    // Exit screen
    if (dynamic_cast<ExitScreen*>(next))
    {
        window_.close();
        return;
    }

    // Start game transition
    if (dynamic_cast<MultiplayerConfigurationScreen*>(currentScreen_) &&
        dynamic_cast<StartScreen*>(next))
    {
        auto* mpScreen = dynamic_cast<MultiplayerConfigurationScreen*>(currentScreen_);
        for (const PlayerConfig* config : mpScreen->getPlayerConfigs()) {
            cachedStats_.push_back(new PlayerStats{
                config->stats.maxBombas,
                config->stats.rangoExplosion,
                config->stats.velocidad
            });
        }

        startGame_ = true;
        return;
    }

    delete currentScreen_;
    currentScreen_ = next;
}

void Vista::transitionToGame() {
    delete currentScreen_;
    currentScreen_ = new GameScreen();
}

void Vista::updateSnapshot(RenderSnapshot& snap){
    GameScreen* gameScreen = dynamic_cast<GameScreen*>(currentScreen_);
    if (gameScreen)
        gameScreen -> update(snap);
}

void Vista::render() {
    if (window_.isOpen()){

        window_.clear(sf::Color::Black);
        currentScreen_ -> render(window_, font_);
        window_.display();
    }
}

std::vector<PlayerStats*> Vista::getPlayerStats() const
{
    return cachedStats_;
}
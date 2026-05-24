#include "rendering/Vista.hpp"
#include "rendering/MainMenuScreen.hpp"
#include "rendering/MultiplayerScreen.hpp"
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

void Vista::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    Screen* next = currentScreen_ -> handleInput(event.key.code);

    if (next != nullptr) {

        if (dynamic_cast<MultiplayerConfigurationScreen*>(currentScreen_) != nullptr &&
            dynamic_cast<StartScreen*>(next) != nullptr) {
            startGame_ = true;
            return;
        }

        if (dynamic_cast<ExitScreen*>(next) != nullptr)
        {
            window_.close();
            return;
        }
       
        delete currentScreen_;
        currentScreen_ = next;
    }
}

void Vista::render() {
    if (window_.isOpen()){

        window_.clear(sf::Color::Black);
        currentScreen_ -> render(window_, font_);
        window_.display();
    }
}

std::vector<PlayerStats> Vista::getPlayerStats() const
{
    std::vector<PlayerStats> info;

    for (PlayerConfig config : dynamic_cast<MultiplayerConfigurationScreen*>(currentScreen_) -> getPlayerConfigs())
    {
        info.push_back(PlayerStats{config.stats.maxBombas, config.stats.rangoExplosion, config.stats.velocidad});
    }

    return info;
}
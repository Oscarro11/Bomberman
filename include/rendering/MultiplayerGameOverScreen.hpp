#pragma once

#include "rendering/Screen.hpp"
#include "utils/ScreenUtils.hpp"
#include <string>

class GameOverScreen : public Screen {
public:
    explicit GameOverScreen(const std::string& winnerName,
                            int                winnerId);

    Screen* handleInput(sf::Keyboard::Key key) override;
    void    render(sf::RenderWindow& window,
                   const sf::Font& font) const override;

private:
    std::string winnerName_;
    int         winnerId_;
    int         selectedOption_ = 0;

    static const sf::Color PLAYER_COLORS[4];
};
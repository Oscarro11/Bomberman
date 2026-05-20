#pragma once

#include "rendering/Screen.hpp"
#include <vector>
#include <string>

struct ScoreEntry {
    std::string name;
    int         score;
    int         wins;
    int         deaths;
};

class ScoresScreen : public Screen {
    public:
        explicit ScoresScreen(const std::vector<ScoreEntry>& scores);

        Screen* handleInput(sf::Keyboard::Key key) override;
        void    render(sf::RenderWindow& window,
                    const sf::Font& font) const override;

    private:
        std::vector<ScoreEntry> scores_;
};
#pragma once

#include "rendering/Screen.hpp"
#include "utils/GameConstants.hpp"
#include "ecs/PlayerStats.hpp"

class MultiplayerConfigurationScreen : public Screen {
    public:
        MultiplayerConfigurationScreen();

        Screen* handleInput(sf::Keyboard::Key key) override;
        void render(sf::RenderWindow& window, const sf::Font& font) const override;

        char keyToChar(sf::Keyboard::Key key) const;

        // Vista reads this when transitioning to the game
        std::vector<const PlayerConfig*> getPlayerConfigs() const;

    private:
        static constexpr int NUM_ATTRS  = 4; // 0=nombre, 1=bombas, 2=rango, 3=vel
        static constexpr int MAX_BOMBAS = 8;
        static constexpr int MIN_BOMBAS = 1;
        static constexpr int MAX_RANGO  = 6;
        static constexpr int MIN_RANGO  = 1;
        static constexpr int MAX_VEL = 5;
        static constexpr int MIN_VEL = 1;

        bool typingName_ = false;
        static constexpr int MAX_NAME_LEN = 5;

        std::vector<PlayerConfig> players_  = std::vector<PlayerConfig>(MIN_PLAYERS);
        int                       activePlayer_ = 0;
        int                       selectedAttr_ = 0;
        int                       selectedOption_ = 0;

        static constexpr int CARD_COLS = 22;
        static constexpr int CARD_HEIGHT = 10;

        void drawPlayerCard(sf::RenderWindow& window, const sf::Font& font,
                                   int playerNum,
                                   const PlayerConfig& cfg, int selectedAttr,
                                   int startRow, int startCol) const;

        void drawAttrLine(sf::RenderWindow&, const sf::Font&,
                      const std::string& label, int value,
                      bool isActivePlayer, bool isActiveAttr,
                      int row, int startCol) const;

        void syncSelectedFlags();
        void incrementAttr(PlayerConfig& p, int attr);
        void decrementAttr(PlayerConfig& p, int attr);
};

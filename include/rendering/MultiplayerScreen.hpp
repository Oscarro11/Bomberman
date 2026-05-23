#pragma once

#include "rendering/Screen.hpp"
#include "rendering/Vista.hpp"

//Se usa en los menus para configurar los stats iniciales de cada jugador
struct PlayerConfig {
    unsigned int vida            = 3;
    unsigned int maxBombas       = 3;
    unsigned int  rangoExplosion = 3;
    double velocidad    = 3.0;
    bool isSelected     = false;   // currently active player slot
};

class MultiplayerConfigurationScreen : public Screen {
    public:
        MultiplayerConfigurationScreen();

        Screen* handleInput(sf::Keyboard::Key key) override;
        void render(sf::RenderWindow& window, const sf::Font& font) const override;

        // Vista reads this when transitioning to the game
        std::vector<PlayerConfig> getPlayerConfigs() const { return players_; }

    private:
        static constexpr int NUM_ATTRS  = 3;
        static constexpr int MAX_BOMBAS = 8;
        static constexpr int MIN_BOMBAS = 1;
        static constexpr int MAX_RANGO  = 6;
        static constexpr int MIN_RANGO  = 1;
        static constexpr double MAX_VEL = 5.0;
        static constexpr double MIN_VEL = 1.0;

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

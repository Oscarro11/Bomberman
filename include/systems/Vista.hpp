#pragma once
#include <SFML/Graphics.hpp>
#include <array>

static const sf::Color COL_BORDER   = sf::Color(80,  80,  80);   // dim gray
static const sf::Color COL_DEFAULT  = sf::Color(200, 200, 200);  // light gray
static const sf::Color COL_SELECTED = sf::Color(232, 232, 48);   // yellow
static const sf::Color COL_VALUE    = sf::Color(80,  250, 123);  // green
static const sf::Color COL_SPRITE   = sf::Color(140, 200, 255);  // cyan

static constexpr int CHAR_W = 10;   // pixels per character (monospaced)
static constexpr int CHAR_H = 18;   // pixels per line
static constexpr int MARGIN = 40;   // left/top margin

static const int MAX_PLAYERS = 4;
static const int MIN_PLAYERS = 2;

//Las diferentes pantallas a usar en el juego, exceptuando la principal
enum class MenuScreen {
    Main,
    SinglePlayer,
    MultiPlayer
};

//Se usa en los menus para configurar los stats iniciales de cada jugador
struct PlayerConfig {
    unsigned int vida            = 3;
    unsigned int maxBombas       = 3;
    unsigned int  rangoExplosion = 3;
    double velocidad    = 3.0;
    bool isSelected     = false;   // currently active player slot
};

class Vista {
    public:
        Vista(sf::RenderWindow& window, sf::Font& font) 
        : window_(window)
        , font_(font)
        , currentScreen_(MenuScreen::Main)
        , startGame_(false)
        {};

        void handleEvent(const sf::Event& event);
        void clear();
        void render();

        //When used, if the game start button is used, it should handle control to Engine
        bool shouldStartGame() const { return startGame_; }
        std::vector<PlayerConfig> get_players_info() const {return players_;};

    private:
        sf::RenderWindow& window_;
        sf::Font&         font_;
        MenuScreen        currentScreen_ = MenuScreen::Main;
        int               selectedOption_ = 0;   // which option in a menu should be highlighted
        bool              startGame_      = false;

        //Helper functions, used in the construction of all 3 screens
        void drawScreen(const std::vector<std::string>& lines);
        void drawLine(const std::string& line, int row, sf::Color color);

        // Each screen is just lines of ASCII text
        std::vector<std::string> buildMainScreen()     const;

        std::vector<std::string> buildSinglePlayerScreen() const;
        
        //Functions for the MultiPlayer start screen
        std::vector<std::string> buildMultiPlayerScreen()  const;
        void syncSelectedFlags();
        void addPlayer();
        void removePlayer();
        int numPlayers() const {return players_.size();}
        std::vector<std::string> buildPlayerCard(int playerNum, const PlayerConfig& cfg, int selectedAttr) const;

        // Info for character selection screen
        std::vector<PlayerConfig> players_ = std::vector<PlayerConfig>(MIN_PLAYERS);
        int activePlayer_ = 0;   // which player slot is selected
        int selectedAttr_ = 0;   // which attribute is being edited by the players
};
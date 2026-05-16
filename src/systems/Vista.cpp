#include "systems/Vista.hpp"
#include <sstream>

void Vista::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    switch (currentScreen_) {
        case MenuScreen::Main:
            if (event.key.code == sf::Keyboard::W)
                selectedOption_ = (selectedOption_ + 2) % 3;   // wrap up
            if (event.key.code == sf::Keyboard::S)
                selectedOption_ = (selectedOption_ + 1) % 3;   // wrap down
            if (event.key.code == sf::Keyboard::Return) {
                if (selectedOption_ == 0) {
                    currentScreen_  = MenuScreen::SinglePlayer;
                    selectedOption_ = 0;
                }
                if (selectedOption_ == 1) {
                    currentScreen_  = MenuScreen::MultiPlayer;
                    selectedOption_ = 0;
                }
                if (selectedOption_ == 2)
                    window_.close();
            }
            break;

        case MenuScreen::SinglePlayer:
            if (event.key.code == sf::Keyboard::Escape)
                currentScreen_ = MenuScreen::Main;
            break;

        case MenuScreen::MultiPlayer:
            // Switch active player
            if (event.key.code == sf::Keyboard::A)
                activePlayer_ = (activePlayer_ + numPlayers() - 1) % numPlayers();
            if (event.key.code == sf::Keyboard::D)
                activePlayer_ = (activePlayer_ + 1) % numPlayers();

            // Cycle attribute being edited
            if (event.key.code == sf::Keyboard::Tab)
                selectedAttr_ = (selectedAttr_ + 1) % 3;

            // Change value of selected attribute
            if (event.key.code == sf::Keyboard::W) {
                auto& p = players_[activePlayer_];
                if (selectedAttr_ == 0) p.maxBombas       = std::min(p.maxBombas + 1,       8);
                if (selectedAttr_ == 1) p.rangoExplosion  = std::min(p.rangoExplosion + 1,  6);
                if (selectedAttr_ == 2) p.velocidad       = std::min(p.velocidad + 1.0,     5.0);
            }
            if (event.key.code == sf::Keyboard::S) {
                auto& p = players_[activePlayer_];
                if (selectedAttr_ == 0) p.maxBombas       = std::max(p.maxBombas - 1,       1);
                if (selectedAttr_ == 1) p.rangoExplosion  = std::max(p.rangoExplosion - 1,  1);
                if (selectedAttr_ == 2) p.velocidad       = std::max(p.velocidad - 1.0,     1.0);
            }
            if (event.key.code == sf::Keyboard::Add) {
                addPlayer();
            }

            if (event.key.code == sf::Keyboard::Subtract) {
                removePlayer();
            }

            syncSelectedFlags();

            if (event.key.code == sf::Keyboard::Return && selectedOption_ == 0)
                startGame_ = true;
            if (event.key.code == sf::Keyboard::Escape)
                currentScreen_ = MenuScreen::Main;
            break;
    }
}

void Vista::render() {
    std::vector<std::string> lines;
    switch (currentScreen_) {
        case MenuScreen::Main:     lines = buildMainScreen();     break;
        case MenuScreen::SinglePlayer:  lines = buildSinglePlayerScreen();  break;
        case MenuScreen::MultiPlayer: lines = buildMultiPlayerScreen(); break;
    }
    drawScreen(lines);
}

void Vista::drawLine(const std::string& line, int row, sf::Color color) {
    sf::Text text;
    text.setFont(font_);
    text.setString(line);
    text.setCharacterSize(14);
    text.setFillColor(color);
    text.setPosition(MARGIN, MARGIN + row * CHAR_H);
    window_.draw(text);
}

void Vista::drawScreen(const std::vector<std::string>& lines) {
    window_.clear(sf::Color::Black);
    for (int i = 0; i < lines.size(); ++i) {
        sf::Color color = COL_DEFAULT;   // default: light gray

        // Highlight the selected option in yellow
        if (lines[i].find(">") != std::string::npos)
            color = COL_SELECTED;            

        // Dim border characters
        if (lines[i].find("+---") != std::string::npos ||
            lines[i].find("|") == 1)
            color = COL_BORDER;

        drawLine(lines[i], i, color);
    }
    window_.display();
}

std::vector<std::string> Vista::buildMainScreen() const {
    return {
        " +---------------------------------------------------------------+",
        " ____                  _                                     ",
        "|  _ \\                | |                                    ",
        "| |_) | ___  _ __ ___ | |__   ___ _ __ _ __ ___   __ _ _ __  ",
        "|  _ < / _ \\| '_ ` _ \\| '_ \\ / _ \\ '__| '_ ` _ \\ / _` | '_ \\ ",
        "| |_) | (_) | | | | | | |_) |  __/ |  | | | | | | (_| | | | |",
        "|____/ \\___/|_| |_| |_|_.__/ \\___|_|  |_| |_| |_|\\__,_|_| |_|",
        " +---------------------------------------------------------------+",
        "",
        "              *** MAIN MENU ***",
        "",
        (selectedOption_ == 0 ? " >  [ SINGLE PLAYER ]  <"
                               : "    [ SINGLE PLAYER ]   "),
        "",
        (selectedOption_ == 1 ? " >  [ MULTI PLAYER ]  <"
                               : "    [ MULTI PLAYER ]   "),
        "",
        (selectedOption_ == 2 ? " >  [   QUIT   ]  <"
                               : "    [   QUIT   ]   "),
        "",
        " use [W/S] to navigate, [ENTER] to select",
    };
}

std::vector<std::string> Vista::buildPlayerCard(
    int playerNum,
    const PlayerConfig& cfg,
    int selectedAttr   // 0=bombas, 1=rango, 2=velocidad — only used if isSelected
) const {
    std::string header = "   P" + std::to_string(playerNum)
                       + (cfg.isSelected ? " (YOU)  " : "        ");

    // Sprite: hand up if selected
    std::string spriteTop = cfg.isSelected ? "      O/     " : "      O      ";
    std::string spriteMid = cfg.isSelected ? "     /|      " : "     /|\\     ";
    std::string spriteBot = "     / \\     ";

    // Each attribute line: highlight the one being edited
    auto attrLine = [&](int attrIdx, const std::string& label, int value) {
        std::string val  = std::to_string(value);
        std::string line = " | " + spriteBot;   // placeholder, overridden below
        if (cfg.isSelected && selectedAttr == attrIdx)
            return " | > " + label + ": " + val + " < |";
        else
            return " |   " + label + ": " + val + "   |";
    };

    return {
        " +---------------+",
        " | " + header + " |",
        " | " + spriteTop + " |",
        " | " + spriteMid + " |",
        " | " + spriteBot + " |",
        " +---------------+",
        attrLine(0, "Bombas", cfg.maxBombas),
        attrLine(1, "Rango ", cfg.rangoExplosion),
        attrLine(2, "Vel   ", (int)cfg.velocidad),
        " +---------------+",
    };
}

std::vector<std::string> Vista::buildSinglePlayerScreen() const {
    return {
        "Single Player Screen :D"
    };
}

std::vector<std::string> Vista::buildMultiPlayerScreen() const {
    std::vector<std::string> screen;

    // Header
    screen.push_back("  +------------------------------------------+");
    screen.push_back("  |           NUEVA PARTIDA                  |");
    screen.push_back("  +------------------------------------------+");
    screen.push_back("");

    // Build each card
    std::vector<std::vector<std::string>> cards;
    for (int i = 0; i < numPlayers(); ++i)
        cards.push_back(buildPlayerCard(i + 1, players_[i], selectedAttr_));
    
    // Merge cards horizontally — zip line by line
    int cardHeight = cards[0].size();
    for (int line = 0; line < cardHeight; ++line) {
        std::string row = "  ";
        for (int p = 0; p < cards.size(); ++p)
            row += cards[p][line] + "  ";
        screen.push_back(row);
    }
    

    screen.push_back("");

    // Navigation hint
    screen.push_back("  [+] add player   [-] remove player   [A/D] switch player   [W/S] change value");
    screen.push_back("  [TAB] next attribute   [ENTER] start game");
    screen.push_back("");
    screen.push_back(selectedOption_ == 0
        ? "  >  [ INICIAR ]  <       [ VOLVER ]"
        : "     [ INICIAR ]    >     [ VOLVER ]  <");

    return screen;
}

void Vista::syncSelectedFlags() {
    for (int i = 0; i < numPlayers(); ++i)
        players_[i].isSelected = (i == activePlayer_);
}

void Vista::addPlayer(){
    if (numPlayers() < MAX_PLAYERS)
    {
        players_.push_back(PlayerConfig{});
        drawScreen(buildMultiPlayerScreen());
    }    
}

void Vista::removePlayer(){
    if (numPlayers() > MIN_PLAYERS)
    {
        players_.pop_back();
        drawScreen(buildMultiPlayerScreen());
    } 
}
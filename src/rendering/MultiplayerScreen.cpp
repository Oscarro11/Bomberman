#include "rendering/MainMenuScreen.hpp"
#include "rendering/MultiplayerScreen.hpp"

MultiplayerConfigurationScreen::MultiplayerConfigurationScreen()
{
    syncSelectedFlags();
}

Screen* MultiplayerConfigurationScreen::handleInput(sf::Keyboard::Key key) {
    switch (key) {

        // --- Player selection ---
        case sf::Keyboard::A:
            activePlayer_ = (activePlayer_ + players_.size() - 1)
                            % players_.size();
            syncSelectedFlags();
            break;

        case sf::Keyboard::D:
            activePlayer_ = (activePlayer_ + 1) % players_.size();
            syncSelectedFlags();
            break;

        // --- Attribute selection ---
        case sf::Keyboard::Tab:
            selectedAttr_ = (selectedAttr_ + 1) % NUM_ATTRS;
            break;

        // --- Change attribute value ---
        case sf::Keyboard::W:
            incrementAttr(players_[activePlayer_], selectedAttr_);
            break;

        case sf::Keyboard::S:
            decrementAttr(players_[activePlayer_], selectedAttr_);
            break;

        // --- Add / remove players ---
        case sf::Keyboard::Q:
            if ((int)players_.size() < MAX_PLAYERS) {
                players_.push_back(PlayerConfig{});
                syncSelectedFlags();
            }
            break;

        case sf::Keyboard::E:
            if ((int)players_.size() > MIN_PLAYERS) {
                players_.pop_back();
                if (activePlayer_ >= (int)players_.size())
                    activePlayer_ = players_.size() - 1;
                syncSelectedFlags();
            }
            break;

        // --- Bottom button selection ---
        case sf::Keyboard::Left:
        case sf::Keyboard::Right:
            selectedOption_ = (selectedOption_ + 1) % 2;
            break;

        // --- Confirm ---
        case sf::Keyboard::Return:
            if (selectedOption_ == 0)
                return new StartScreen;    // → launches Engine
            if (selectedOption_ == 1)
                return new MainMenuScreen();  // → back to main

        // --- Back ---
        case sf::Keyboard::Escape:
            return new MainMenuScreen();

        default:
            break;
    }

    return Screen::STAY;
}

void MultiplayerConfigurationScreen::render(sf::RenderWindow& window,
                           const sf::Font& font) const {
    using namespace ScreenUtils;

    int row = 0;

    // Header
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    drawLine(window, font, " |              NUEVA PARTIDA                    |", row++, COL_ACCENT);
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    row++;

    // Player count control
    drawSegments(window, font, {
        { " players: ",                          COL_DEFAULT  },
        { "<",                                  COL_SELECTED },
        { std::to_string(players_.size()),        COL_VALUE    },
        { ">",                                  COL_SELECTED },
        { "   [Q] aniadir    [E] quitar",             COL_DIM      },
    }, row++);
    row++;

    // Player cards side by side
    // Each card is CARD_COLS characters wide — offset each by that amount
    int cardStartRow = row;
    for (int i = 0; i < (int)players_.size(); ++i)
        drawPlayerCard(window, font, i, players_[i], selectedAttr_,
                       cardStartRow, i * CARD_COLS);

    // Advance past the card height
    row = cardStartRow + CARD_HEIGHT;
    row++;

    // Navigation hints
    drawLine(window, font, " [A/D] switch player   [W/S] change value", row++, COL_DIM);
    drawLine(window, font, " [TAB] next attribute   [ESC] back",         row++, COL_DIM);
    row++;

    // Bottom buttons
    drawSegments(window, font, {
        { "  ",                                           COL_DEFAULT  },
        { selectedOption_ == 0 ? "> [ INICIAR ] <"
                               : "  [ INICIAR ]  ",      selectedOption_ == 0
                                                        ? COL_SELECTED : COL_DEFAULT },
        { "      ",                                       COL_DEFAULT  },
        { selectedOption_ == 1 ? "> [ VOLVER ]  <"
                               : "  [ VOLVER ]   ",      selectedOption_ == 1
                                                        ? COL_SELECTED : COL_DEFAULT },
    }, row);
}

void MultiplayerConfigurationScreen::drawPlayerCard(sf::RenderWindow& window,
                                   const sf::Font& font,
                                   int playerNum,
                                   const PlayerConfig& cfg,
                                   int selectedAttr,
                                   int startRow,
                                   int startCol) const {
    using namespace ScreenUtils;

    sf::Color headerColor  = cfg.isSelected ? COL_SELECTED : COL_DEFAULT;
    sf::Color spriteColor  = cfg.isSelected ? COL_ACCENT   : COL_DIM;
    int row = startRow;

    // Border + header
    drawLineAt(window, font, " +----------------+", row++, startCol, COL_BORDER);
    drawLineAt(window, font,
        " | P" + std::to_string(playerNum + 1)
        + (cfg.isSelected ? "  < ACTIVE > |" : "             |"),
        row++, startCol, headerColor);
    drawLineAt(window, font, " +----------------+", row++, startCol, COL_BORDER);

    // Sprite
    drawLineAt(window, font,
        cfg.isSelected ? " |       O/       |"
                       : " |       O        |",
        row++, startCol, spriteColor);
    drawLineAt(window, font, 
        cfg.isSelected ? " |      /|        |"
                       : " |      /|\\       |",
        row++, startCol, spriteColor);
    drawLineAt(window, font, " |      / \\       |", row++, startCol, spriteColor);
    drawLineAt(window, font, " +----------------+", row++, startCol, COL_BORDER);

    // Attribute lines — mixed color per segment
    drawAttrLine(window, font, "Bombas", cfg.stats.maxBombas,
                 cfg.isSelected, selectedAttr == 0,
                 row++, startCol);
    drawAttrLine(window, font, "Rango ", cfg.stats.rangoExplosion,
                 cfg.isSelected, selectedAttr == 1,
                 row++, startCol);
    drawAttrLine(window, font, "Vel   ", (int)cfg.stats.velocidad,
                 cfg.isSelected, selectedAttr == 2,
                 row++, startCol);

    drawLineAt(window, font, " +----------------+", row, startCol, COL_BORDER);
}

void MultiplayerConfigurationScreen::drawAttrLine(sf::RenderWindow& window,
                                  const sf::Font& font,
                                  const std::string& label,
                                  int value,
                                  bool isActivePlayer,
                                  bool isActiveAttr,
                                  int row, int startCol) const {
    using namespace ScreenUtils;

    sf::Color labelColor = !isActivePlayer ? COL_DIM
                         : isActiveAttr    ? COL_SELECTED : COL_DEFAULT;
    sf::Color valueColor = !isActivePlayer ? COL_DIM
                         : isActiveAttr    ? COL_VALUE    : COL_DEFAULT;

    bool active = isActiveAttr && isActivePlayer;

    // Pad label to fixed width so value always starts at same x
    std::string paddedLabel = label;
    paddedLabel.resize(6, ' ');   // "Bombas" = 6, "Rango " = 6, "Vel   " = 6

    drawSegmentsAt(window, font, {
        { " | ",          COL_BORDER   },
        { active ? "> " : "  ", COL_SELECTED },
        { paddedLabel + ": ", labelColor },
        { std::to_string(value), valueColor },
        { active ? " <" : "  ", COL_SELECTED },
        { "  |",           COL_BORDER   },
    }, row, startCol);
}

void MultiplayerConfigurationScreen::syncSelectedFlags() {
    for (int i = 0; i < (int)players_.size(); ++i)
        players_[i].isSelected = (i == activePlayer_);
}

void MultiplayerConfigurationScreen::incrementAttr(PlayerConfig& p, int attr) {
    switch (attr) {
        case 0: p.stats.maxBombas      = std::min(p.stats.maxBombas + 1,      (unsigned int) MAX_BOMBAS);  break;
        case 1: p.stats.rangoExplosion = std::min(p.stats.rangoExplosion + 1,  (unsigned int) MAX_RANGO);  break;
        case 2: p.stats.velocidad      = std::min(p.stats.velocidad + 1.0,     MAX_VEL);    break;
    }
}

void MultiplayerConfigurationScreen::decrementAttr(PlayerConfig& p, int attr) {
    switch (attr) {
        case 0: p.stats.maxBombas      = std::max(p.stats.maxBombas - 1,      (unsigned int) MIN_BOMBAS); break;
        case 1: p.stats.rangoExplosion = std::max(p.stats.rangoExplosion - 1, (unsigned int) MIN_RANGO); break;
        case 2: p.stats.velocidad      = std::max(p.stats.velocidad - 1.0,    MIN_VEL);   break;
    }
}

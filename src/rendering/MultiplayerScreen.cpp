#include "rendering/MainMenuScreen.hpp"
#include "rendering/MultiplayerScreen.hpp"
#include "rendering/GameScreen.hpp"

#include "utils/GameConstants.hpp"
#include "ecs/PlayerStats.hpp"

MultiplayerConfigurationScreen::MultiplayerConfigurationScreen()
{
    syncSelectedFlags();
}

char MultiplayerConfigurationScreen::keyToChar(sf::Keyboard::Key key) const {
    // Letters
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
        return 'A' + (key - sf::Keyboard::A);

    // Numbers
    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
        return '0' + (key - sf::Keyboard::Num0);

    return 0;   // non-printable — ignore
}

Screen* MultiplayerConfigurationScreen::handleInput(sf::Keyboard::Key key) {
    // ── Typing mode — capture name characters ──────────
    if (typingName_) {
        PlayerConfig& p = players_[activePlayer_];

        if (key == sf::Keyboard::Return || key == sf::Keyboard::Tab) {
            // Confirm name
            typingName_    = false;
            selectedAttr_  = 1;   // move to next attr after confirming
            return Screen::STAY;
        }

        if (key == sf::Keyboard::BackSpace) {
            if (!p.stats.nombre.empty())
                p.stats.nombre.pop_back();
            return Screen::STAY;
        }

        if (key == sf::Keyboard::Escape) {
            typingName_ = false;
            return Screen::STAY;
        }

        // Append printable character if within length limit
        if ((int)p.stats.nombre.size() < MAX_NAME_LEN) {
            char c = keyToChar(key);
            if (c != 0)
                p.stats.nombre += c;
        }

        return Screen::STAY;
    }

    // ── Normal navigation ───────────────────────────────
    switch (key) {
        case sf::Keyboard::A:
            activePlayer_ = (activePlayer_ + players_.size() - 1) % players_.size();
            syncSelectedFlags();
            break;

        case sf::Keyboard::D:
            activePlayer_ = (activePlayer_ + 1) % players_.size();
            syncSelectedFlags();
            break;

        case sf::Keyboard::Tab:
            selectedAttr_ = (selectedAttr_ + 1) % NUM_ATTRS;
            break;

        case sf::Keyboard::W:
            if (selectedAttr_ == 0) break;   // name not changed with W/S
            incrementAttr(players_[activePlayer_], selectedAttr_);
            break;

        case sf::Keyboard::S:
            if (selectedAttr_ == 0) break;
            decrementAttr(players_[activePlayer_], selectedAttr_);
            break;

        // Enter on name field → start typing
        case sf::Keyboard::Return:
            if (selectedOption_ == 0 && selectedAttr_ == 0) {
                typingName_ = true;
                break;
            }
            if (selectedOption_ == 0 && selectedAttr_ != 0)
                return new StartScreen();
            if (selectedOption_ == 1)
                return new MainMenuScreen();
            break;

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

        case sf::Keyboard::Left:
        case sf::Keyboard::Right:
            selectedOption_ = (selectedOption_ + 1) % 2;
            break;

        case sf::Keyboard::Escape:
            return new MainMenuScreen();

        default: break;
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
    row += 2;

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

std::vector<const PlayerConfig*> MultiplayerConfigurationScreen::getPlayerConfigs() const
{
    std::vector<const PlayerConfig*> result;

    for (const PlayerConfig& config : players_)
    {
        result.push_back(&config);
    }

    return result;
}

void MultiplayerConfigurationScreen::drawPlayerCard(sf::RenderWindow &window,
                                                    const sf::Font &font,
                                                    int playerNum,
                                                    const PlayerConfig &cfg,
                                                    int selectedAttr,
                                                    int startRow,
                                                    int startCol) const
{
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

    // Name of player
    // Name field — fixed MAX_NAME_LEN width
    bool nameActive  = cfg.isSelected && selectedAttr == 0;
    bool nameTyping  = nameActive && typingName_;

    // Build fixed-width display string
    std::string nameDisplay = cfg.stats.nombre;
    if (nameTyping) nameDisplay += "_";          // append cursor while typing
    nameDisplay.resize(MAX_NAME_LEN, ' ');       // pad or truncate to fixed width

    drawSegmentsAt(window, font, {
        { " | ",                     COL_BORDER              },
        { nameActive ? "> " : "  ",  COL_SELECTED            },
        { "Name: ",                  nameActive
                                ? COL_SELECTED : COL_DIM  },
        { nameDisplay,               nameTyping  ? COL_VALUE
                                : nameActive  ? COL_DEFAULT
                                                : COL_DIM   },
        { nameActive ? " <" : "  ",  COL_SELECTED            },
        { "  |",                     COL_BORDER              },
    }, row++, startCol);

    // Attribute lines — mixed color per segment
    drawAttrLine(window, font, "Bombas", cfg.stats.maxBombas,
                 cfg.isSelected, selectedAttr == 1,
                 row++, startCol);
    drawAttrLine(window, font, "Rango ", cfg.stats.rangoExplosion,
                 cfg.isSelected, selectedAttr == 2,
                 row++, startCol);

    drawLineAt(window, font, " +----------------+", row++, startCol, COL_BORDER);
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
    paddedLabel.resize(6, ' ');   // "Bombas" = 6, "Rango " = 6

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
        case 1: p.stats.maxBombas      = std::min(p.stats.maxBombas + 1,      ( int) MAX_BOMBAS);  break;
        case 2: p.stats.rangoExplosion = std::min(p.stats.rangoExplosion + 1,  ( int) MAX_RANGO);  break;
    }
}

void MultiplayerConfigurationScreen::decrementAttr(PlayerConfig& p, int attr) {
    switch (attr) {
        case 1: p.stats.maxBombas      = std::max(p.stats.maxBombas - 1,      ( int) MIN_BOMBAS); break;
        case 2: p.stats.rangoExplosion = std::max(p.stats.rangoExplosion - 1, ( int) MIN_RANGO); break;
    }
}

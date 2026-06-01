#include "rendering/MainMenuScreen.hpp"
#include "rendering/InstructionsScreen.hpp"
#include "rendering/ScoresScreen.hpp"
#include "rendering/MultiplayerScreen.hpp"
#include "rendering/SingleplayerScreen.hpp"
#include "rendering/GameScreen.hpp"

Screen *MainMenuScreen::handleInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W)
        selectedOption_ = (selectedOption_ + 4) % 5;
    if (key == sf::Keyboard::S)
        selectedOption_ = (selectedOption_ + 1) % 5;
    
    if (key == sf::Keyboard::Return) {
        if (selectedOption_ == 0) return new InstructionsScreen;
        if (selectedOption_ == 1)
        {
            //Example scores, should be removed later
            std::vector<ScoreEntry> scores = 
                {ScoreEntry{"Ana", 2000, 1, 2},
                 ScoreEntry{"Pablo", 2500, 3, 3},
                 ScoreEntry{"Juan", 4000, 3, 2},
                 ScoreEntry{"Mateo", 6000, 5, 3}};

            return new ScoresScreen(scores);
        }
            
        if (selectedOption_ == 2) return new SinglePlayerConfigurationScreen;
        if (selectedOption_ == 3) return new MultiplayerConfigurationScreen;    //Multi player mode, just a preview
        if (selectedOption_ == 4) return new ExitScreen;   // quit handled by Vista
    }
    
    return Screen::STAY;   // stay on this screen
}

void MainMenuScreen::render(sf::RenderWindow &window, const sf::Font &font) const
{
    using namespace ScreenUtils;
    
    drawLine(window, font, " +---------------------------------------------------------------+", 0, COL_BORDER);
    drawLine(window, font, " ____                  _                                     ", 1, COL_DEFAULT);
    drawLine(window, font, "|  _ \\                | |                                    ", 2, COL_DEFAULT);
    drawLine(window, font, "| |_) | ___  _ __ ___ | |__   ___ _ __ _ __ ___   __ _ _ __  ", 3, COL_DEFAULT);
    drawLine(window, font, "|  _ < / _ \\| '_ ` _ \\| '_ \\ / _ \\ '__| '_ ` _ \\ / _` | '_ \\ ", 4, COL_DEFAULT);
    drawLine(window, font, "| |_) | (_) | | | | | | |_) |  __/ |  | | | | | | (_| | | | |", 5, COL_DEFAULT);
    drawLine(window, font, "|____/ \\___/|_| |_| |_|_.__/ \\___|_|  |_| |_| |_|\\__,_|_| |_|", 6, COL_DEFAULT);
    drawLine(window, font, " +---------------------------------------------------------------+", 7, COL_BORDER);
    drawLine(window, font, "", 8, COL_BORDER);
    drawLine(window, font, "              *** MAIN MENU ***", 9, COL_ACCENT);
    drawLine(window, font, "", 10, COL_BORDER);

    std::string optInstructions = selectedOption_ == 0 ? " >  [   INSTRUCTIONS   ]  <" : "    [   INSTRUCTIONS   ]   ";
    std::string optScores = selectedOption_ == 1 ? " >  [   SCORES   ]  <" : "    [   SCORES   ]   ";
    std::string optSinglePlayer = selectedOption_ == 2 ? " >  [   SINGLEPLAYER   ]  <" : "    [   SINGLEPLAYER   ]   ";
    std::string optMultiPlayer = selectedOption_ == 3 ? " >  [   MULTIPLAYER   ]  <" : "    [   MULTIPLAYER   ]   ";
    std::string optQuit = selectedOption_ == 4 ? " >  [   QUIT   ]  <" : "    [   QUIT   ]   ";
    
    drawLine(window, font, optInstructions, 11, selectedOption_ == 0 ? COL_SELECTED : COL_DEFAULT);
    drawLine(window, font, optScores, 12, selectedOption_ == 1 ? COL_SELECTED : COL_DEFAULT);
    drawLine(window, font, optSinglePlayer, 13, selectedOption_ == 2 ? COL_SELECTED : COL_DEFAULT);
    drawLine(window, font, optMultiPlayer, 14, selectedOption_ == 3 ? COL_SELECTED : COL_DEFAULT);
    drawLine(window, font, optQuit, 15, selectedOption_ == 4 ? COL_SELECTED : COL_DEFAULT);

    drawLine(window, font, " use [W/S] to navigate, [ENTER] to select", 16, COL_DEFAULT);
}

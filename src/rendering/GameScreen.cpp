#include "rendering/GameScreen.hpp"
#include "rendering/MainMenuScreen.hpp"

GameScreen::GameScreen() {
    this -> tablero_ = new Tablero("/home/oscar/bomberman/assets/mapas/mapa.txt");
}

Screen* GameScreen::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape || key == sf::Keyboard::Return)
        return new MainMenuScreen;
    
    return nullptr;
}

void GameScreen::render(sf::RenderWindow& window, const sf::Font& font) const {
    int row = -1;
    using namespace ScreenUtils;

    for (const std::vector<CellContent> lista : tablero_ -> getBoard()){
        row++;
        std::vector<TextSegment> segmentos;

        for (const CellContent celda : lista)
        {
            switch (celda.type)
            {
                case Player1: segmentos.push_back(TextSegment{"@", sf::Color(252, 3, 211)}); break;
                case Player2: segmentos.push_back(TextSegment{"$", sf::Color(17, 5, 245)}); break;
                case Enemy: segmentos.push_back(TextSegment{"1", sf::Color(237, 9, 9)}); break;
                case Bomb: segmentos.push_back(TextSegment{"!", sf::Color(245, 225, 2)}); break;
                case Explosion: segmentos.push_back(TextSegment{"0", sf::Color(245, 164, 2)}); break;
                case Wall: segmentos.push_back(TextSegment{"#", sf::Color(130, 120, 120)}); break;
                case DestructibleWall: segmentos.push_back(TextSegment{"+", sf::Color(94, 44, 30)}); break;
                case Hole: segmentos.push_back(TextSegment{"/", sf::Color(0, 0, 0)}); break;
                case Floor: segmentos.push_back(TextSegment{".", sf::Color(44, 232, 7)}); break;
                case PowerUpItem: segmentos.push_back(TextSegment{"?", sf::Color(11, 222, 208)}); break;
                
                default: break;
            }
        };

        drawSegments(window, font, segmentos, row);
    };

    drawLine(window, font, "\n\n", row + 1, COL_DEFAULT);
    drawLine(window, font, "Jugador 1: ", row + 4, COL_DEFAULT);
    drawLine(window, font, "    Bombas: 3", row + 5, COL_DEFAULT);
    drawLine(window, font, "    Rango: 3", row + 6, COL_DEFAULT);
    drawLine(window, font, "    Velocidad : 3", row + 7, COL_DEFAULT);
    drawLine(window, font, " ", row + 8, COL_DEFAULT);
    drawLine(window, font, "Jugador 2: ", row + 9, COL_DEFAULT);
    drawLine(window, font, "    Bombas: 3", row + 10, COL_DEFAULT);
    drawLine(window, font, "    Rango: 3", row + 11, COL_DEFAULT);
    drawLine(window, font, "    Velocidad: 3", row + 12, COL_DEFAULT);
}
#include "rendering/InstructionsScreen.hpp"
#include "rendering/MainMenuScreen.hpp"

using namespace ScreenUtils;

Screen* InstructionsScreen::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape || key == sf::Keyboard::Return)
        return new MainMenuScreen;
    return Screen::STAY;
}

void InstructionsScreen::render(sf::RenderWindow& window,
                                 const sf::Font& font) const {
    int row = 0;

    // Header
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    drawLine(window, font, " |             INSTRUCCIONES                     |", row++, COL_ACCENT);
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    row++;

    // Controls
    drawLine(window, font, "  CONTROLES",                                  row++, COL_ACCENT);
    drawLine(window, font, "  P1: W A S D  |  bomba: [SPACE]",             row++, COL_DEFAULT);
    drawLine(window, font, "  P2: ^ < v >  |  bomba: [ENTER]",             row++, COL_DEFAULT);
    row++;

    // Map symbols
    drawLine(window, font, "  SIMBOLOS",                                   row++, COL_ACCENT);
    drawLine(window, font, "  # pared indestructible",                   row++, COL_DEFAULT);
    drawLine(window, font, "  + bloque destruible",                      row++, COL_DEFAULT);
    drawLine(window, font, "  ! bomba activa",                           row++, COL_VALUE);
    drawLine(window, font, "  0 explosion",                             row++, COL_SELECTED);
    drawLine(window, font, "  ? power up",                               row++, COL_VALUE);
    row++;

    // Rules
    drawLine(window, font, "  REGLAS",                                     row++, COL_ACCENT);
    drawLine(window, font, "  - El objetivo del juego es derrotar a tus oponentes y ser el ultimo en pie. ", row++, COL_DEFAULT);
    drawLine(window, font, "    Para ello, tu puedes colocar bombas en el tablero que los danien. Eso si, ten cuidado:", row++, COL_DEFAULT);
    drawLine(window, font, "    Las bombas tambien pueden daniarte a ti! ", row++, COL_DEFAULT);
    row++;

    // Extra details
    drawLine(window, font, "  NOTAS",   row++, COL_ACCENT);
    drawLine(window, font, "  - Una vez coloques una bomba, esta no puede reposicionarse de ninguna manera",   row++, COL_DEFAULT);
    drawLine(window, font, "  - Las explosiones destruyen algunos bloques [+] y",   row++, COL_DEFAULT);
    drawLine(window, font, "    eliminan jugadores que entren en contacto con ellas.",      row++, COL_DEFAULT);
    drawLine(window, font, "  - Una explosion puede detonar otra bomba, creando una reaccion en cadena.",  row++, COL_DEFAULT);
    drawLine(window, font, "  - El ultimo jugador en pie gana la ronda.",  row++, COL_DEFAULT);
    row++;

    // Footer
    drawLine(window, font, " +-----------------------------------------------+", row++, COL_BORDER);
    drawLine(window, font, "  [ESC] / [ENTER]  volver al menu",            row,   COL_DIM);
}
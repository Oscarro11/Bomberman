#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace ScreenUtils {

    // Layout constants
    extern int CHAR_W;
    extern int CHAR_H;
    extern int MARGIN;
    extern double CHAR_SCALE;

    // Color palette
    extern const sf::Color COL_BORDER;
    extern const sf::Color COL_DEFAULT;
    extern const sf::Color COL_SELECTED;
    extern const sf::Color COL_VALUE;
    extern const sf::Color COL_ACCENT;
    extern const sf::Color COL_DIM;

    // Segment for mixed-color lines
    struct TextSegment {
        std::string text;
        sf::Color   color;
    };

    // Function to set the size of rendered chars, based on window size
    void setCharSize(sf::RenderWindow& window);

    // Drawing functions
    void drawLine(sf::RenderWindow& window, const sf::Font& font,
                  const std::string& line, int row, sf::Color color);

    void drawLineAt(sf::RenderWindow& window, const sf::Font& font,
                    const std::string& line, int row, int col,
                    sf::Color color);

    void drawSegments(sf::RenderWindow& window, const sf::Font& font,
                      const std::vector<TextSegment>& segments, int row);

    void drawSegmentsAt(sf::RenderWindow& window, const sf::Font& font,
                    const std::vector<TextSegment>& segments,
                    int row, int startCol);
}
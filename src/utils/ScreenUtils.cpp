#include "utils/ScreenUtils.hpp"
#include "math.h"

namespace ScreenUtils {

    // Color definitions
    const sf::Color COL_BORDER   = sf::Color(80,  80,  80);
    const sf::Color COL_DEFAULT  = sf::Color(200, 200, 200);
    const sf::Color COL_SELECTED = sf::Color(232, 232, 48);
    const sf::Color COL_VALUE    = sf::Color(80,  250, 123);
    const sf::Color COL_ACCENT   = sf::Color(255, 165, 0);
    const sf::Color COL_DIM      = sf::Color(50,  50,  50);

    //Starting values for character rendering, will be updated with setCharSize
    int CHAR_W = 0;
    int CHAR_H = 0;
    int MARGIN = 40;
    double CHAR_SCALE = 1.0;

    void setCharSize(sf::RenderWindow& window)
    {
        sf::Vector2u windowSize = window.getSize();
        
        CHAR_W = windowSize.x / 60;
        CHAR_H = windowSize.y / 30;
        MARGIN = 40;
        CHAR_SCALE = sqrt(pow(windowSize.x, 2) + pow(windowSize.y, 2)) / 1000; //1000 is an estimation, could be changed if necessary
    }

    void drawLine(sf::RenderWindow& window, const sf::Font& font,

        const std::string& line, int row, sf::Color color) {
        sf::Text text;
        text.setFont(font);
        text.setString(line);
        text.setCharacterSize((unsigned int) 14 * CHAR_SCALE);
        text.setFillColor(color);
        text.setPosition(MARGIN, MARGIN + row * CHAR_H);
        window.draw(text);
    }

    void drawLineAt(sf::RenderWindow& window, const sf::Font& font,
                    const std::string& line, int row, int col,
                    sf::Color color) {
        sf::Text text;
        text.setFont(font);
        text.setString(line);
        text.setCharacterSize((unsigned int) 14 * CHAR_SCALE);
        text.setFillColor(color);
        text.setPosition(MARGIN + col * CHAR_W, MARGIN + row * CHAR_H);
        window.draw(text);
    }

    void drawSegments(sf::RenderWindow& window, const sf::Font& font,
                      const std::vector<TextSegment>& segments, int row) {
        float x = MARGIN;
        float y = MARGIN + row * CHAR_H;

        for (const auto& seg : segments) {
            sf::Text text;
            text.setFont(font);
            text.setString(seg.text);
            text.setCharacterSize((unsigned int) 14 * CHAR_SCALE);
            text.setFillColor(seg.color);
            text.setPosition(x, y);
            window.draw(text);

            x += seg.text.size() * CHAR_W;
        }
    }

    void drawSegmentsAt(sf::RenderWindow& window, const sf::Font& font,
                    const std::vector<TextSegment>& segments,
                    int row, int startCol) {
        float x = MARGIN + startCol * CHAR_W;
        float y = MARGIN + row * CHAR_H;

        for (const auto& seg : segments) {
            sf::Text text;
            text.setFont(font);
            text.setString(seg.text);
            text.setCharacterSize((unsigned int) 14 * CHAR_SCALE);
            text.setFillColor(seg.color);
            text.setPosition(x, y);
            window.draw(text);
            x += seg.text.size() * CHAR_W;
        }
    }
}
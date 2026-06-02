#include "utils/ScreenUtils.hpp"
#include <cmath>

namespace ScreenUtils {

    const sf::Color COL_BORDER   = sf::Color( 80,  80,  80);
    const sf::Color COL_DEFAULT  = sf::Color(200, 200, 200);
    const sf::Color COL_SELECTED = sf::Color(232, 232,  48);
    const sf::Color COL_VALUE    = sf::Color( 80, 250, 123);
    const sf::Color COL_ACCENT   = sf::Color(255, 165,   0);
    const sf::Color COL_DIM      = sf::Color( 50,  50,  50);

    int    CHAR_W     = 0;
    int    CHAR_H     = 0;
    float  CHAR_W_F   = 0.f;
    int    MARGIN     = 40;
    double CHAR_SCALE = 1.0;

    void setCharSize(sf::RenderWindow& window) {
        sf::Vector2u windowSize = window.getSize();
        CHAR_H     = windowSize.y / 30;
        MARGIN     = 40;
        CHAR_SCALE = std::sqrt(std::pow((float)windowSize.x, 2.f)
                             + std::pow((float)windowSize.y, 2.f)) / 1000.f;
    }

    void measureFont(const sf::Font& font) {
        int charSize = (int)(14.f * CHAR_SCALE);

        CHAR_W_F = font.getGlyph('M', charSize, false).advance;
        CHAR_W   = static_cast<int>(CHAR_W_F);
    }

    void drawLine(sf::RenderWindow& window, const sf::Font& font,
                  const std::string& line, int row, sf::Color color) {
        sf::Text text;
        text.setFont(font);
        text.setString(line);
        text.setCharacterSize((int)(14.f * CHAR_SCALE));
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
        text.setCharacterSize((int)(14.f * CHAR_SCALE));
        text.setFillColor(color);
        text.setPosition(MARGIN + col * CHAR_W_F, MARGIN + row * CHAR_H);
        window.draw(text);
    }

    void drawSegments(sf::RenderWindow& window, const sf::Font& font,
                      const std::vector<TextSegment>& segments, int row) {
        float x = (float)MARGIN;
        float y = (float)(MARGIN + row * CHAR_H);
        int cursor = 0;

        for (const auto& seg : segments) {
            if (seg.text.empty()) continue;
            sf::Text text;
            text.setFont(font);
            text.setString(seg.text);
            text.setCharacterSize((int)(14.f * CHAR_SCALE));
            text.setFillColor(seg.color);
            text.setPosition(x + cursor * CHAR_W_F, y);
            window.draw(text);

            cursor += seg.text.size();
        }
    }

    void drawSegmentsAt(sf::RenderWindow& window, const sf::Font& font,
                        const std::vector<TextSegment>& segments,
                        int row, int startCol) {
        float x = MARGIN + startCol * CHAR_W_F;
        float y = (float)(MARGIN + row * CHAR_H);
        int cursor = 0;

        for (const auto& seg : segments) {
            if (seg.text.empty()) continue;
            sf::Text text;
            text.setFont(font);
            text.setString(seg.text);
            text.setCharacterSize((int)(14.f * CHAR_SCALE));
            text.setFillColor(seg.color);
            text.setPosition(x + cursor * CHAR_W_F, y);
            window.draw(text);
            
            cursor += seg.text.size();
        }
    }
}
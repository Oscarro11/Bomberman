#include "input/InputHandler.hpp"

//Mapa de teclas a usar para cada jugador
const KeyMap InputHandler::KEYMAPS[MAX_PLAYERS] = {
    { sf::Keyboard::W, sf::Keyboard::S,
      sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::E },   // P1
    { sf::Keyboard::Up, sf::Keyboard::Down,
      sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::RShift },  // P2
    { sf::Keyboard::I,  sf::Keyboard::K,
      sf::Keyboard::J,  sf::Keyboard::L,     sf::Keyboard::O },  // P2
    { sf::Keyboard::Numpad8, sf::Keyboard::Numpad5,
      sf::Keyboard::Numpad4, sf::Keyboard::Numpad6, sf::Keyboard::Numpad7 } // P3
};

InputHandler::InputHandler(IEngine *engine)
    : engine_(engine)
{
}

void InputHandler::update(std::vector<sf::Keyboard::Key> pressedKeys){
    for (const sf::Keyboard::Key &key: pressedKeys)
    {
        // Check every mapped key across all players
        for (int id = 0; id < MAX_PLAYERS; ++id) {
            const KeyMap& km = KEYMAPS[id];

            if (key == km.up || key == km.down || key == km.left || key == km.right || key == km.bomb){
                    engine_ -> handleInput(key, id);
                }
        }
    } 
}
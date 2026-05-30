#include "input/InputHandler.hpp"

//Mapa de teclas a usar para cada jugador
const KeyMap InputHandler::KEYMAPS[MAX_PLAYERS] = {
    {
        sf::Keyboard::Key::W,
        sf::Keyboard::Key::S,
        sf::Keyboard::Key::A,
        sf::Keyboard::Key::D,
        sf::Keyboard::Key::E
    }, // P1

    {
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::Down,
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::RShift
    }, // P2

    {
        sf::Keyboard::Key::I,
        sf::Keyboard::Key::K,
        sf::Keyboard::Key::J,
        sf::Keyboard::Key::L,
        sf::Keyboard::Key::O
    }, // P3

    {
        sf::Keyboard::Key::Numpad8,
        sf::Keyboard::Key::Numpad5,
        sf::Keyboard::Key::Numpad4,
        sf::Keyboard::Key::Numpad6,
        sf::Keyboard::Key::Numpad7
    } // P4
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
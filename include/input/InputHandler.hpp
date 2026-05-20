#pragma once

#include "entities/Player.hpp"
#include "rendering/Vista.hpp"
#include "utils/Directions.hpp"
#include "core/IEngine.hpp"
#include <set>
#include <atomic>

struct KeyMap
{
    sf::Keyboard::Key up, down, left, right, bomb;
};

//Se usa para procesar el ingreso de inputs junto con quien le pertenece dicho input
struct InputInfo
{
  sf::Keyboard::Key key;
  int playerId;

  //Necesario para la funcion de find
  bool operator<(const InputInfo& other) const {
        if (playerId != other.playerId)
            return playerId < other.playerId;

        return key < other.key;
  };
};

class InputHandler {
    public:
        InputHandler(IEngine *engine);
        void update(std::vector<sf::Keyboard::Key> pressedKeys);

        // One KeyMap per player — index matches playerId
        static const KeyMap KEYMAPS[MAX_PLAYERS];

    private:
        
        sf::Time updateTime_ = sf::seconds(0);
        IEngine*             engine_;
        pthread_t            thread_;
        std::atomic<bool>    running_;
};



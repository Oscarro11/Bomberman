#pragma once
#include "ecs/Evento.hpp"

//This file is used to avoid a circular dependency between IutHandler and Engine
class IEngine {
public:
    virtual void pushEvento(const Evento& e) = 0;
    virtual void handleInput(sf::Keyboard::Key key, int playerId) = 0;
    virtual int  numPlayers()          const = 0;
    virtual bool running()             const = 0;
    virtual ~IEngine() = default;
};
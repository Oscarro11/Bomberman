#include <cassert>
#include "ecs/Evento.hpp"
#include "entities/Player.hpp"

#include <iostream>

//These test requiere that the Evento constructor is public
void runTest(){
    Player player = Player(1, 3, 3, 3, 7, 5, 5);

    std::optional<Evento> temp = player.colocarBomba();
    Evento eventManual = Evento::manualEvent(EventType::PlayerPlaceBomb, 1, -1, 5, 5, {});
    Evento event = Evento::manualEvent(EventType::BombExplode, 0, 0, 0, 0, {});

    if (temp.has_value())
    {
        event = temp.value();
    } 

    assert(eventManual.autor() == event.autor());
    assert(eventManual.posicionX() == event.posicionX());
    assert(eventManual.posicionY() == event.posicionY());
    assert(eventManual.objetivo() == event.objetivo());
    assert(eventManual.tipo() == event.tipo());
}

int main() {
    runTest();

    printf("PlayerTest se completo con exito\n");
    return 0;
}
#include <cassert>
#include "ecs/Evento.hpp"
#include "entities/Player.hpp"

//These test requiere that the Evento constructor is public
void runTest(){
    Player player = Player(1, 3, 3, 5.f, 5, 7);
    Evento event = player.colocarBomba();
    Evento eventManual = Evento::manualEvent(EventType::PlayerPlaceBomb, 1, -1, 5, 7, {});

    assert(eventManual.autor() == event.autor());
    assert(eventManual.posicionX() == event.posicionX());
    assert(eventManual.posicionY() == event.posicionY());
    assert(eventManual.objetivo() == event.objetivo());
    assert(eventManual.tipo() == event.tipo());
}

int main() {
    runTest();
    return 0;
}
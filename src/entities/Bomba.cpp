#include "entities/Bomba.hpp"
#include "core/Engine.hpp"

Bomba::Bomba(int id, unsigned int posX, unsigned int posY,
             unsigned int radio, unsigned int timerMs,
             Engine* engine, sem_t* semBombas)
    : id_(id), posX_(posX), posY_(posY), radio_(radio), 
      timerMs_(timerMs), engine_(engine), semBombas_(semBombas)
{
    // El constructor solo inicializa — NO lanza el hilo
    // El hilo se lanza después con iniciar()
}

void Bomba::iniciar()
{
    // pthread_create(puntero_al_thread, atributos, funcion, argumento)
    pthread_create(&thread_, NULL, threadFunc, (void*)this);
}

void* Bomba::threadFunc(void* arg)
{
    // Recuperamos quién somos
    Bomba* yo = (Bomba*)arg;
    
    // DUERME la bomba durante su tiempo de detonación
    // sf::sleep toma sf::milliseconds, es de SFML
    sf::sleep(sf::milliseconds(yo->timerMs_));
    
    // Generamos el evento de explosión
    Evento explosion = Evento::bombExplode(
        yo->id_,        // id de quien causa la explosión
        yo->posX_,      // posición X
        yo->posY_,      // posición Y
        yo->radio_      // radio de la explosión
    );
    
    // Lo mandamos al Engine para que lo procese en el hilo principal
    yo->engine_->pushEvento(explosion);
    
    // Liberamos el semáforo — le decimos al jugador que puede colocar otra bomba
    sem_post(yo->semBombas_);
    
    // El hilo termina aquí
    return nullptr;
}
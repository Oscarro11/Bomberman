#pragma once  //esto le dice al compilador que solo incluya este archivo una vez
#include <pthread.h> //hilo de la bomba
#include <semaphore.h> // para controlar el limite de bombas por jugador
#include "ecs/Evento.hpp" //para manejar los eventos relacionados con la bomba

class Engine;  // "tanto engine como bomba se necesitan mutuamente, entonces se declara la clase engine antes de incluir su header"

class Bomba {
    int id_; // Identificador único de la bomba
    unsigned int posX_; //columna donde esta la bomba
    unsigned int posY_; //fila donde esta la bomba
    unsigned int radio_; //cuantas casillas alcanza la explosion
    unsigned int timerMs_; //milisegundos para que la bomba explote
    Engine* engine_; //para poder hacer pushEvento()
    sem_t* semBombas_; //semaforo para controlar el limite de bombas por jugador
    pthread_t thread_; //hilo propio de la bomba

//Funcion que ejecuta el hilo, debe ser estatica porque pthread no puede llamar funciones de instancia directamente
    static void* threadFunc(void* arg);

public:
    Bomba(int id, unsigned int posX, unsigned int posY, unsigned int radio,
        unsigned int timerMs, Engine* engine, sem_t* semBombas);
        void iniciar(); //inicia el hilo de la bomba
};



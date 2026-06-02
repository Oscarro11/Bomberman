#pragma once

#include <queue>
#include <optional>
#include <pthread.h>

#include "ecs/Evento.hpp"

class EventBus {
    public:
        EventBus();
        ~EventBus();

        void push(const Evento& evento);

        std::optional<Evento> pop();

        void stop();

    private:
        std::queue<Evento> queue_;

        pthread_mutex_t mutex_;
        pthread_cond_t eventReady_;

        bool running_;
};
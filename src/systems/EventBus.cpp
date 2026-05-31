#include "systems/EventBus.hpp"

EventBus::EventBus()
    : running_(true)
{
    pthread_mutex_init(&mutex_, nullptr);
    pthread_cond_init(&eventReady_, nullptr);
}

EventBus::~EventBus()
{
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&eventReady_);
}

void EventBus::push(const Evento& evento)
{
    pthread_mutex_lock(&mutex_);

    queue_.push(evento);

    pthread_cond_signal(&eventReady_);
    pthread_mutex_unlock(&mutex_);
}

std::optional<Evento> EventBus::pop()
{
    pthread_mutex_lock(&mutex_);
    while (queue_.empty() && running_)
    {
        pthread_cond_wait(&eventReady_, &mutex_);
    }

    if (!running_)
    {
        pthread_mutex_unlock(&mutex_);
        return std::nullopt;
    }

    Evento evento = queue_.front();
    queue_.pop();

    pthread_mutex_unlock(&mutex_);

    return evento;
}

void EventBus::stop()
{
    pthread_mutex_lock(&mutex_);
    running_ = false;
    pthread_cond_broadcast(&eventReady_);
    pthread_mutex_unlock(&mutex_);
}
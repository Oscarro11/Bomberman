#pragma once

#include <optional>

#include "entities/Personaje.hpp"
#include "utils/Position.hpp"
#include <SFML/System.hpp>

//Forward declaration
enum PowerUpType : int;

class Player : public Personaje
{
    private:
        int maxBombas_;
        int restBombas_;
        int rangoExplosion_;
        int velocidad_;
        int spawnPointX_;
        int spawnPointY_; 
        bool invencible_;
        sf::Clock invencibleClock;

    public:
        Player(int id, int vida, int maxBombas, int rangoExplosion, int velocidad, int spawnPointX, int spawnPointY);
        virtual ~Player() = default;

        Evento generarEventoMov(Directions direction) override;
        std::optional<Evento> recibirDanio(const Personaje& atacante) override;

        std::optional<Evento> colocarBomba();
        void actualizarStat(PowerUpType tipo, int cantidad);
        void recuperarBomba();

        bool esInvencible() const;
        void activarInvencibilidad();
        void actualizarInvencibilidad();

        int maxBombas() const {return maxBombas_;};
        int restBombas() const {return restBombas_;};
        int rangoExplosion() const {return rangoExplosion_;};
        int velocidad() const {return velocidad_;};
        int spawnPointX() const {return spawnPointX_;};
        int spawnPointY() const {return spawnPointY_;};
};

#pragma once

#include <optional>
#include <SFML/System.hpp>

#include "entities/Personaje.hpp"
#include "utils/Position.hpp"

//Forward declaration
enum PowerUpType : int;

class Player : public Personaje
{
    private:
        std::string nombre_;
        int puntaje_;

        int maxBombas_;
        int restBombas_;
        int rangoExplosion_;
        int velocidad_;

        int spawnPointX_;
        int spawnPointY_;

        bool invencible_;
        sf::Clock invencibleClock;

    public:
        Player(int id, std::string nombre, int vida, int maxBombas, int rangoExplosion, int velocidad, int spawnPointX, int spawnPointY);
        virtual ~Player() = default;

        Evento generarEventoMov(Directions direction) override;
        std::optional<Evento> recibirDanio(const Personaje& atacante) override;

        std::optional<Evento> colocarBomba();
        void actualizarStat(PowerUpType tipo, int cantidad);

        bool esInvencible() const;
        void activarInvencibilidad();
        void actualizarInvencibilidad();

        std::string nombre() const {return nombre_;}
        int maxBombas() const {return maxBombas_;};
        int restBombas() const {return restBombas_;};
        int rangoExplosion() const {return rangoExplosion_;};
        int velocidad() const {return velocidad_;};
        int spawnPointX() const {return spawnPointX_;};
        int spawnPointY() const {return spawnPointY_;};
};

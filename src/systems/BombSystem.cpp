#include "systems/BombSystem.hpp"

BombSystem::BombSystem(std::vector<Player>& players,
            std::vector<Bomba>& bombs,
            std::vector<Explosion>& explosions,
            std::vector<Enemigo>& enemies,
            Tablero& tablero,
            EventBus& bus)
            : players_(players)
            , bombs_(bombs)
            , explosions_(explosions)
            , enemies_(enemies)
            , tablero_(tablero)
            , bus_(bus)
{}

void BombSystem::update(sf::Time dt)
{
    for (auto it = bombs_.begin();
        it != bombs_.end();)
    {
        if (it->expirada())
        {
            bus_.push(
                Evento::bombExplode(
                    it->getId(),
                    it->getCreador(),
                    it->getX(),
                    it->getY(),
                    it->getRadio()
                )
            );

            it = bombs_.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = explosions_.begin();
        it != explosions_.end();)
    {
        if (it->expirada())
        {
            tablero_.removeOccupant(
                Position{it->getX(), it->getY()},
                EntityType::Explosion,
                it->getId()
            );

            it = explosions_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool BombSystem::placeBomb(
    int creatorId,
    int x,
    int y,
    int range)
{
    int bombId = nextBombId_++;

    Bomba bomb(
        bombId,
        creatorId,
        x,
        y,
        range,
        sf::seconds(3.f)
    );

    bombs_.push_back(bomb);

    tablero_.addOccupant(
        Position{x, y},
        Occupant{
            EntityType::Bomb,
            bombId
        }
    );

    return true;
}

void BombSystem::explodeBomb(const Evento& evento)
{
    int creador = evento.data().explosion.creador;

    Position pos{
        evento.posicionX(),
        evento.posicionY()
    };

    int bombId = evento.autor();

    tablero_.removeOccupant(
        pos,
        EntityType::Bomb,
        bombId
    );

    createExplosion(pos, creador);

    int radio = evento.data().explosion.radio;
    explodeDirection(pos, creador,  1,  0, radio); // derecha
    explodeDirection(pos, creador, -1,  0, radio); // izquierda
    explodeDirection(pos, creador,  0, -1, radio); // arriba
    explodeDirection(pos, creador,  0,  1, radio); // abajo
}

void BombSystem::createExplosion(Position pos, int creador)
{
    int explosionId = nextExplosionId_++;

    explosions_.push_back(
        Explosion(
            explosionId,
            pos.x,
            pos.y,
            sf::seconds(2.f), //Duracion de la explosion, pendiente de ajustar
            creador
        )
    );

    BoardCell cell = tablero_.getCell(pos);

    for (const Occupant& occ : cell.occupants)
    {   
        if (occ.type == EntityType::Enemy)
        {
            Enemigo& enemigo = enemies_[occ.entityId];
            auto deathEvent = enemigo.recibirDanio(players_[creador]);

            if (deathEvent.has_value())
            {
                bus_.push(deathEvent.value());
            }
        }

        if (occ.type == EntityType::Player1 ||
            occ.type == EntityType::Player2 ||
            occ.type == EntityType::Player3 ||
            occ.type == EntityType::Player4)
        {
            bus_.push(Evento::playerDamage(creador, occ.entityId, pos.x, pos.y));
        }

        if (occ.type == EntityType::Bomb)
        {
            bus_.push(Evento::chainExplosion(occ.entityId));
        }
    }

    tablero_.addOccupant(
        pos,
        Occupant{
            EntityType::Explosion,
            explosionId
        }
    );
}

void BombSystem::explodeDirection(Position origen, int creador, int dx, int dy, int radio)
{
    for (int i = 1; i <= radio; i++)
    {
        Position p{
            origen.x + dx * i,
            origen.y + dy * i
        };

        if (p.x < 0 ||
            p.y < 0 ||
            p.x >= tablero_.getWidth() ||
            p.y >= tablero_.getHeight())
        {
            break;
        }

        BoardCell cell = tablero_.getCell(p);
        
        if (cell.terrainType == TileType::Wall)
        {
            break;
        }

        createExplosion(p, creador);

        if (cell.terrainType == TileType::Breakable)
        {
            bus_.push(Evento::tileDestroyed(p.x, p.y));
            break;
        }
    }
}

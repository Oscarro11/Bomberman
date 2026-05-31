#include "systems/EnemySystem.hpp"

EnemySystem::EnemySystem(
    std::vector<Enemigo>& enemies,
    EventBus& bus,
    Tablero& tablero)
    : enemies_(enemies)
    , bus_(bus)
    , tablero_(tablero)
    , timer_(sf::Time::Zero)
{}

void EnemySystem::update(sf::Time dt)
{
    timer_ += dt;

    if (timer_ < ENEMY_MOVE_INTERVAL)
        return;

    timer_ = sf::Time::Zero;

    for (Enemigo& enemy : enemies_)
    {
        Directions direction;
        int num = rand() % 4;

        switch (num){
            case 0: direction = Directions::DOWN; break;
            case 1: direction = Directions::UP; break;
            case 2: direction = Directions::LEFT; break;
            case 3: direction = Directions::RIGHT; break;
            default: break;
        }

        Evento e = enemy.generarEventoMov(direction);

        bus_.push(e);
    }
}

std::optional<int> EnemySystem::processMove(const Evento& evento)
{
    int enemyId = evento.autor();

    if (enemyId < 0 ||
        enemyId >= static_cast<int>(enemies_.size()))
    {
        return std::nullopt;
    }

    Enemigo& enemy = enemies_[enemyId];

    int dx = evento.data().mover.dx;
    int dy = evento.data().mover.dy;

    Position oldPos{
        enemy.posX(),
        enemy.posY()
    };

    Position newPos{
        oldPos.x + dx,
        oldPos.y + dy
    };

    const BoardCell cell = tablero_.getCell(newPos);

    for (const Occupant& occ : cell.occupants)
    {
        if (occ.type == EntityType::Player1 ||
            occ.type == EntityType::Player2 ||
            occ.type == EntityType::Player3 ||
            occ.type == EntityType::Player4)
        {
            tablero_.moveOccupant(
                oldPos,
                newPos,
                enemyId);

            enemy.setPosition(
                newPos.x,
                newPos.y);
            
            return occ.entityId;
        }
    }

    if (!tablero_.isWalkable(newPos))
        return std::nullopt;

    bool moved =
        tablero_.moveOccupant(
            oldPos,
            newPos,
            enemyId);

    if (!moved)
        return std::nullopt;

    enemy.setPosition(
        newPos.x,
        newPos.y);
    
    return std::nullopt;
}
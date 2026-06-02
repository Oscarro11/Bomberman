#include "systems/EnemySystem.hpp"

EnemySystem::EnemySystem(
    std::vector<Enemigo>& enemies,
    Tablero& tablero,
    EventBus& bus)
    : enemies_(enemies)
    , bus_(bus)
    , tablero_(tablero)
    , timer_(sf::Time::Zero)
{
    pthread_mutex_init(&enemyMutex_, nullptr);
}

EnemySystem::~EnemySystem()
{
    pthread_mutex_destroy(&enemyMutex_);
}

void EnemySystem::update(sf::Time dt)
{
    timer_ += dt;

    if (timer_ < ENEMY_MOVE_INTERVAL)
        return;

    timer_ = sf::Time::Zero;
    std::vector<Evento> events;

    pthread_mutex_lock(&enemyMutex_);
    for (Enemigo& enemy : enemies_)
    {
        if (enemy.alive())
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

            events.push_back(enemy.generarEventoMov(direction));
        }  
    }
    pthread_mutex_unlock(&enemyMutex_);

    for (auto& e : events)
    {
        bus_.push(e);
    }
}

std::optional<int> EnemySystem::processMove(const Evento& evento)
{
    int enemyId = evento.autor();

    int dx = evento.data().mover.dx;
    int dy = evento.data().mover.dy;

    pthread_mutex_lock(&enemyMutex_);
    if (enemyId < 0 ||
        enemyId >= static_cast<int>(enemies_.size()))
    {
        pthread_mutex_unlock(&enemyMutex_);
        return std::nullopt;
    }

    Enemigo& enemy = enemies_[enemyId];

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
            
            pthread_mutex_unlock(&enemyMutex_);
            return occ.entityId;
        }
    }

    if (!tablero_.isWalkable(newPos))
    {
        pthread_mutex_unlock(&enemyMutex_);
        return std::nullopt;
    }        

    bool moved =
        tablero_.moveOccupant(
            oldPos,
            newPos,
            enemyId);

    if (!moved)
    {
        pthread_mutex_unlock(&enemyMutex_);
        return std::nullopt;
    }
    
    enemy.setPosition(
        newPos.x,
        newPos.y);
    
    pthread_mutex_unlock(&enemyMutex_);
    return std::nullopt;
}

void EnemySystem::killEnemy(int enemyId)
{
    pthread_mutex_lock(&enemyMutex_);

    if (enemyId >= 0 &&
        enemyId < static_cast<int>(enemies_.size()))
    {
        enemies_[enemyId].kill();
    }

    pthread_mutex_unlock(&enemyMutex_);
}

bool EnemySystem::hasLivingEnemies()
{
    pthread_mutex_lock(&enemyMutex_);

    bool result = false;

    for (const Enemigo& enemy : enemies_)
    {
        if (enemy.alive())
        {
            result = true;
            break;
        }
    }

    pthread_mutex_unlock(&enemyMutex_);

    return result;
}
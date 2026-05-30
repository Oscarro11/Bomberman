#pragma once

enum EventType{
    PlayerMove,
    PlayerPlaceBomb,
    PlayerDeath,
    PlayerPickPowerUp,
    BombExplode,
    ChainExplosion,
    EnemyMove,
    EnemyDeath,
    TileDestroyed,
    GameOver,
    RoundStart
};

union EventData{
    struct {int dx, dy;} mover;
    struct {int radio;} explosion;
    struct {int tipo;} powerUp;
};

class Evento
{
    //For testing purposes, these attributes are protected. TODO: change this to private
    private:
        EventType tipo_;
        int autor_, objetivo_, posicionX_, posicionY_;
        EventData data_;    

        //Constructor privado, usado para fabricas
        Evento(EventType tipo, int autor, int objetivo, int posicionX, int posicionY, EventData data);

    public:
        ~Evento() = default;

        //Constructores de fabrica
        static Evento playerMove(int autorId, int posX, int posY, int dx, int dy);
        static Evento playerPlaceBomb(int autorId, int posX, int posY);
        static Evento playerDeath(int autorId, int playerId, int posX, int posY);
        static Evento playerPickPowerUp(int autorId, int powerUpId, int posX, int posY, int powerUpTipo);
        
        /*
        static Evento bombExplode(int bombId,  int posX,  int posY, int radio);
        static Evento chainExplosion(int bombId);
        static Evento enemyMove(int enemyId, int dx, int dy);
        static Evento enemyDeath(int enemyId,  int posX,  int posY);
        static Evento tileDestroyed(int posX, int posY);
        static Evento gameOver();
        static Evento roundStart(); 
        */

        //Development method, must be removed for production
        static Evento manualEvent(EventType tipo, int autor, int objetivo, int posicionX, int posicionY, EventData data);

        //Accesores
        EventType tipo()      const { return tipo_; }
        int       autor()     const { return autor_; }
        int       objetivo()  const { return objetivo_; }
        int       posicionX() const { return posicionX_; }
        int       posicionY() const { return posicionY_; }
        const EventData& data()    const { return data_; }
    
};
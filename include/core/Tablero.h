#ifndef TABLERO_H
#define TABLERO_H

#include <vector>
#include <string>

#include "Player.h"
#include "Enemigo.h"
#include "PowerUp.h"
#include "Bomba.h"

using namespace std;

class Tablero
{
private:

    vector<vector<int>> tablero;

    vector<Player> listaPlayers;

    vector<Enemigo> listaEnemigos;

    vector<Bomba> listaBombas;

    vector<PowerUp> listaPowerUps;

public:

    Tablero(string source);

    int getCell(int row, int column);

    void setCell(int id, int row, int column);

    int getRows();

    int getColumns();

    vector<vector<int>> getBoard();
};

#endif
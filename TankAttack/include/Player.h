#pragma once
#include "tank.h"

class Player {
private:

	//ATRIBUTOS
	int playerID;
	bool isMyTurn;
	Tank* tanks[2];

public:
	// constructor y destructor
	Player(int playerID, Tank* tank1, Tank* tank2, bool isMyTurn);
	~Player();

	//Metodos publicos
	int getID();

	bool isItMyTurn();
	void setTurn(bool value);

	Tank* getTank(int index);

};
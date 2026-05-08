#pragma once
#include "tank.h"

class Player {
private:

	//ATRIBUTOS
	int playerID;
	bool isMyTurn;
	Tank* tanks[2];

	//Tanque seleccionado actualmente
	Tank* selectedTank = nullptr; 

public:
	// constructor y destructor
	Player(int playerID, Tank* tank1, Tank* tank2, bool isMyTurn);
	~Player();

	//Metodos publicos
	int getID();

	bool isItMyTurn();
	void setTurn(bool value);
	void selectTank(Tank* tank);
		
	Tank* getTank(int index);
	Tank* getSelectedTank();

};
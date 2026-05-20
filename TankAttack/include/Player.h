#pragma once
#include "tank.h"
#include "PowerUp.h"

class Player {
private:

	//ATRIBUTOS
	int playerID;
	Tank* tanks[2];
	//Tanque seleccionado actualmente
	Tank* selectedTank = nullptr; 

	PowerUp* powerUp;

public:
	// constructor y destructor
	Player(int playerID, Tank* tank1, Tank* tank2, float powerUpX, float powerUpY, sf::RenderWindow* window, sf::Vector2u windowSize);
	~Player();

	//Metodos publicos
	int getID();

	void selectTank(Tank* tank);
	void deselectTank();
	int tanksAlive();
		
	Tank* getTank(int index);
	Tank* getSelectedTank();

	//metodos para los powerUp del jugador
	void addRandomPowerUp();
	bool hasPowerUp();
	void usePowerUp();
	int getActivePowerUp();
	void clearActivePowerUp();
	void drawPowerUp();
};
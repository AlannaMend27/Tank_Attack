#include "Player.h"

// constructor y destructor

Player::Player(int playerID, Tank* tank1, Tank* tank2, bool isMyTurn)
{
	this->playerID = playerID;
	this->tanks[0] = tank1;
	this->tanks[1] = tank2;
	this->isMyTurn = isMyTurn;
}

Player::~Player()
{
	delete this->powerUp;
}

//Metodos publicos

int Player::getID()
{
	return this->playerID;
}

bool Player::isItMyTurn() 
{
	return this->isMyTurn;
}

//Activa o quita el turno a este jugador
void Player::setTurn(bool value)
{
	this->isMyTurn = value;
}

void Player::selectTank(Tank* tank) 
{
	//Si ya esta seleccionado, quitarlo
	if (this->selectedTank == tank) {
		this->selectedTank = nullptr;
		return;
	}

	this->selectedTank = tank;
}

void Player::deselectTank()
{
	this->selectedTank = nullptr;
}

//Para saber donde esta el tanque 1 o 2 del jugador x
Tank* Player::getTank(int index) 
{
	return this->tanks[index];
}

Tank* Player::getSelectedTank()
{
	return this->selectedTank;
}

PowerUp* Player::getPowerUp() 
{
	return this->powerUp;
}
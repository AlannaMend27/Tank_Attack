#include "Player.h"

// constructor y destructor

Player::Player(int playerID, Tank* tank1, Tank* tank2, float powerUpX, float powerUpY, sf::RenderWindow* window, sf::Vector2u windowSize)
{
	this->playerID = playerID;
	this->tanks[0] = tank1;
	this->tanks[1] = tank2;
	this->powerUp = new PowerUp(window, windowSize, powerUpX, powerUpY);
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

void Player::selectTank(Tank* tank) 
{
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

int Player::tanksAlive() {
	int cont = 0;
	for (int i = 0; i < 2; i++) {
		if (this->tanks[i]->getIsAlive()) {
			cont++;
		}
	}
	return cont;
}

Tank* Player::getSelectedTank()
{
	return this->selectedTank;
}

// metodos relacionados al power up del jugador

void Player::addRandomPowerUp() 
{ 
	this->powerUp->addRandom(); 
}

bool Player::hasPowerUp() 
{ 
	return this->powerUp->hasPowerUp(); 
}

void Player::usePowerUp() 
{ 
	this->powerUp->usePowerUp(); 
}

int Player::getActivePowerUp() 
{ 
	return this->powerUp->getActivePowerUp(); 
}

void Player::clearActivePowerUp() 
{ 
	this->powerUp->clearActivePowerUp(); 
}

void Player::drawPowerUp() 
{ 
	this->powerUp->drawPowerUp(); 
}
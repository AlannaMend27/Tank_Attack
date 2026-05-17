#pragma once
#include <SFML/Graphics.hpp>
#include "GameConfig.h"
#include "Queue.h"



// tipos de power up
// nota: esto lo podemos cambiar y no usar enum, seria hacerlo en gameConfig con un id para cada poder, pero es menos legible
// tipo en los ifs seria if (powerup==2) en vez de if (powerup==doubleturn) digamos
enum class PowerUpType {
	doubleTurn, 
	movementPrecision, 
	attackPrecision, 
	attackPower
};

class PowerUp {
private: 
	//ATRIBUTOS

	//cola de poderes del jugador
	Queue* powerUpQueue;
	//-1 si no hay ninguno
	int activePowerUp;
	float iconPosX;

	sf::Font font;
	sf::Text powerUpCount;

	sf::Texture doubleTurnTexture;
	sf::Texture movementPrecisionTexture;
	sf::Texture attackPrecisionTexture;
	sf::Texture	attackPowerTexture;

	sf::RenderWindow* window;
	sf::Vector2u windowSize;


	//metodos privados
	void renderIcons();

public:
	//constructor y destructor
	PowerUp(sf::RenderWindow* window, sf::Vector2u windowSize, float iconPosX);
	~PowerUp();

	//Metodos publicos
	void addRandom();
	void usePowerUp();
	void drawPowerUp();
	
	int getActivePowerUp();

	bool hasPowerUp();

	void clearActivePowerUp();
};

#pragma once
#include <SFML/Graphics.hpp>

class Tank {
private:

	//ATRIBUTOS
	int currentRow;
	int currentCol;

	sf::Vector2u windowSize;
	sf::RenderWindow* window;

	std::string texturePath; //Por que son 4 texturas
	sf::Texture tankTexture;
	sf::Sprite tankSprite;

	//metodos privados
	void initTank();
	void renderTank();

public:
	//Constructor y destructor
	Tank(int row, int col, sf::Vector2u windowSize, sf::RenderWindow* window, std::string texturePath);
	~Tank();

	//Metodos publicos
	void createTank(); 
	int getCurrentRow();
	int getCurrentCol();

};
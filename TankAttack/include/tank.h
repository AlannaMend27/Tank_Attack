#pragma once
#include <SFML/Graphics.hpp>

class Tank {
private:

	//ATRIBUTOS 
	
	//posicion logica en el mapa
	int row;
	int col;

	//para calcular el tamaño del tanque 
	sf::Vector2u windowSize;
	sf::RenderWindow* window;

	//Sprites del tenque
	sf::Texture tank_texture;
	sf::Sprite tank_sprite;
	

public:

	//Constructor y destructor
	Tank(int row, int col, sf::Vector2u windowSize,sf::RenderWindow* window, std::string tankTexturePath);
	~Tank();

	//Metodos publicos de momento

	void createTank();

	//Retornan la posicion del tanque
	int getRow();
	int getCol();

};


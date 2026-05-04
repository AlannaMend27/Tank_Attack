#pragma once
#include <SFML/Graphics.hpp>

class Tank {
private:
	//ATRIBUTOS 

	//posicion en el mapa
	int row;
	int col;

	sf::Sprite tank_sprite;

public: 

	//Constructor y destructor
	Tank(int row, int col, float cellWidth, float cellHeight, sf::Texture& texture);
	~Tank();


	//Metodos publicos
	void renderTank(sf::RenderWindow* window);

	//Retornan la posicion del tanque
	int getRow();
	int getCol();


};

#include "tank.h"


//Constructor y destructor

Tank::Tank(int row, int col, sf::Vector2u windowSize,sf::RenderWindow* window , std::string tankTexturePath) {

	//Posicion logica 
	this->row = row;
	this->col = col;

	this->windowSize = windowSize;
	this->window = window;

	this->tank_texture.loadFromFile(tankTexturePath);
	this->tank_sprite.setTexture(this->tank_texture);

	

}

Tank::~Tank()
{
	
}

//Metodos publicos de momento


void Tank::createTank(){

	float cellWidth = (float)this->windowSize.x / 15;
	float cellHeight = (float)this->windowSize.y / 15;

	this->tank_sprite.setScale(cellWidth / this->tank_texture.getSize().x, cellHeight / this->tank_texture.getSize().y);
	this->tank_sprite.setPosition(this->col / cellWidth, this->row * cellHeight);

	this->window->draw(this->tank_sprite);
}


//Obtienen la posicion en columna y fila del tanque 

int Tank::getCol() {
	return this->col;
}

int Tank::getRow() {
	return this->row;
}


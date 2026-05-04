#include "tank.h"

//Constructor y destructor
Tank::Tank(int row, int col, sf::Vector2u windowSize, sf::RenderWindow* window, std::string texturePath) 
{
	//Posicion logica del tanque
	this->currentRow = row;
	this->currentCol = col;

	//Para ajustar el tamanio 
	this->windowSize = windowSize;
	this->window = window;

	this->texturePath = texturePath;
	this->initTank();

}

Tank::~Tank() 
{
	
}

//Metodos privados

void Tank::initTank() 
{
	//cargar textura y asignar el sprite
	this->tankTexture.loadFromFile(this->texturePath);
	this->tankSprite.setTexture(this->tankTexture);

}

void Tank::renderTank()
{
	//para saber el tamano de cada celda
	float cellWidth = (float)this->windowSize.x / 15;
	float cellHeight = (float)this->windowSize.y / 15;

	//NOTA: ESE 15 HAY QUE CAMBIARLO SI SE CAMBIA MAPSIZE , SE PUEDE HACER UN .H DE CONSTANTS PARA ESO, AYUDARIA EN MAPA TAMBIEN

	//se escala el tamano del sprite a las celdas
	this->tankSprite.setScale(cellWidth / this->tankTexture.getSize().x, cellHeight / this->tankTexture.getSize().y);
	this->tankSprite.setPosition(this->getCurrentCol() * cellWidth, this->getCurrentRow() * cellHeight);

	this->window->draw(this->tankSprite);

}

//Metodos publicos

void Tank::createTank() 
{
	this->renderTank();
}

//Esto es para la posicion logica, la actual

int Tank::getCurrentRow() 
{
	return this->currentRow;
}

int Tank::getCurrentCol() 
{
	return this->currentCol;
}
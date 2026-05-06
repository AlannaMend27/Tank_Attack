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
	float cellWidth = (float)this->windowSize.x / MAP_SIZE;
	float cellHeight = (float)this->windowSize.y / MAP_SIZE;

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
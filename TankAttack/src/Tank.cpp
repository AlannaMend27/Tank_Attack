#include "tank.h"
#include <iostream>

//Constructor y destructor
Tank::Tank(int row, int col, sf::Vector2u windowSize, sf::RenderWindow* window, std::string texturePath, std::string id)
{
	//Posicion logica del tanque
	this->currentRow = row;
	this->currentCol = col;

	//Para ajustar el tamanio 
	this->windowSize = windowSize;
	this->window = window;
	this->texturePath = texturePath;
	this->cellHeight = (float)this->windowSize.y / MAP_SIZE;
	this->cellWidth = (float)this->windowSize.x / MAP_SIZE;

	// posicion visual (para movimiento mientras no se ha llegado al destino para establecer la posicion logica)
	this->visualX = this->currentCol * this->cellWidth;
	this->visualY = this->currentRow * this->cellHeight;

	// movimiento del tanque 
	this->pathToGo = nullptr;
	this->isMoving = false;
	this->pathIndex = -1;
	this->pathSize = 0;

	// id (color) del tanque
	this->tankID = id;

	this->initTank();

}

Tank::~Tank()
{
	delete[] pathToGo;
}

// METODOS PRIVADOS

void Tank::initTank()
{
	//cargar textura y asignar el sprite
	this->tankTexture.loadFromFile(this->texturePath);
	this->tankSprite.setTexture(this->tankTexture);

	// inicializar posiciones visuales al crear el tanque
	this->visualX = this->currentCol * this->cellWidth;
	this->visualY = this->currentRow * this->cellHeight;
	this->tankSprite.setPosition(this->visualX, this->visualY);


}

void Tank::renderTank()
{
	//se escala el tamano del sprite a las celdas
	this->tankSprite.setScale(this->cellWidth / this->tankTexture.getSize().x, this->cellHeight / this->tankTexture.getSize().y);
	this->window->draw(this->tankSprite);

}

// METODOS PUBLICOS

// moveSprite actualiza la posición visual antes de llegar a una celda destino y actualizar la posicion logica
void Tank::moveSprite(float dx, float dy) {
	this->visualX += dx;
	this->visualY += dy;
	this->tankSprite.setPosition(this->visualX, this->visualY);
}

// setPosition también actualiza la posición visual definitiva cuando se llega a una celda destino
void Tank::setPosition(float x, float y) {
	this->visualX = x;
	this->visualY = y;
	this->tankSprite.setPosition(x, y);
}

// cuando se crea el tanque, se renderiza
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

// retorna el id que se le asigno al tanque
std::string Tank::getId()
{
	return this->tankID;
}

// sets y get del path
// establece el camino a seguir e indica que nos vamos a mover siempre que la variable isMoving sea true
void Tank::setPathToGo(int* path, int sizePath)
{
	this->pathToGo = path;
	this->pathSize = sizePath;
	this->pathIndex = 0;
	this->isMoving = true;
}

int* Tank::getPathToGo()
{
	return this->pathToGo;
}

// get del tamanio el path
int Tank::GetPathSize()
{
	return this->pathSize;
}

// incrementar y get de el indice del path al que ha avanzado el tanque
void Tank::incrementPathIndex()
{
	this->pathIndex++;
}

int Tank::getPathIndex()
{
	return this->pathIndex;
}

// get y set de la variable IsMoving
bool Tank::getIsMoving()
{
	return this->isMoving;
}

void Tank::setIsMoving(bool value)
{
	this->isMoving = value;
}

sf::Vector2f Tank::getSpritePosition()
{
	return this->tankSprite.getPosition();
}

void Tank::setCurrentRow(int row)
{
	this->currentRow = row;
}

void Tank::setCurrentCol(int col)
{
	this->currentCol = col;
}


void Tank::clearPath()
{
	delete[] this->pathToGo;
	this->pathToGo = nullptr;
	this->pathIndex = 0;
	this->pathSize = 0;

}

//Esto obtiene los limites del tanque en un FloatRect para hacer .getBounds, es lo mismo que usan los botones
//Sirve para el rango de seleccion de un tanque al hacer click
sf::FloatRect Tank::getArea() 
{
	return this->tankSprite.getGlobalBounds();
}

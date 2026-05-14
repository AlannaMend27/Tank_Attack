#include "Bullet.h"

// constructor y destructor
Bullet::Bullet(int row, int col, sf::Vector2u windowSize, sf::RenderWindow* window) 
{
	this->currentRow = row;
	this->currentCol = col;
	this->window = window;
	this->cellWidth = (float)windowSize.x / MAP_SIZE;
	this->cellHeight = (float)windowSize.y / MAP_SIZE;

	//posicion visial inicial
	this->visualX = this->currentCol * this->cellWidth;
	this->visualY = this->currentRow * this->cellHeight;

	//movimiento de la bala
	this->pathToGo = nullptr;
	this->pathSize = 0;
	this->pathIndex = 0;
	this->isMoving = false;

	this->initBullet();
}

Bullet::~Bullet()
{
	delete[] this->pathToGo;
}

// METODOS PRIVADOS

void Bullet::initBullet()
{
	// nota: este sprite es por mientras, no parece la de un tanque jajjaj
	this->bulletTexture.loadFromFile("assets/textures/bullet.png");
	this->bulletSprite.setTexture(this->bulletTexture);

	// para que se vea mas pequenia que un tanque
	this->bulletSprite.setScale(this->cellWidth / this->bulletTexture.getSize().x, this->cellHeight / this->bulletTexture.getSize().y);

	// centrarla en la celda
	this->visualX = this->currentCol * this->cellWidth;
	this->visualY = this->currentRow * this->cellHeight;
	this->bulletSprite.setPosition(this->visualX, this->visualY);
}

void Bullet::renderBullet()
{
	//actualizar el sprite y dibyujarlo
	this->bulletSprite.setPosition(this->visualX, this->visualY);
	this->window->draw(this->bulletSprite);

	//(nota para mi) aqui va el path de las balas

}

//METODOS PUBLICOS

//cuando se crea la bala se renderiza
void Bullet::createBullet()
{
	this->renderBullet();
}

// sets y get del path
//establece el camino a seguir (igual que el del tanque)
void Bullet::setPathToGo(int* path, int pathSize)
{
	this->pathToGo = path;
	this->pathSize = pathSize;
	this->pathIndex = 0;
	this->isMoving = true;
}

int* Bullet::getPathToGo()
{
	return this->pathToGo;
}


int Bullet::GetPathSize()
{
	return this->pathSize;
}

int Bullet::getPathIndex()
{
	return this->pathIndex;
}

void Bullet::incrementPathIndex()
{
	this->pathIndex++;
}

//get y set de is moving
bool Bullet::getIsMoving()
{
	return this->isMoving;
}

void Bullet::setIsMoving(bool value)
{
	this->isMoving = value;
}

sf::Vector2f Bullet::getSpritePosition()
{
	return this->bulletSprite.getPosition();
}

void Bullet::moveSprite(float dx, float dy)
{
	this->visualX += dx;
	this->visualY += dy;
	// nota: aqui podemos verificar si la bala toco un tanque comparando posicion logica
}

void Bullet::setPosition(float x, float y)
{
	this->visualX = x;
	this->visualY = y;
	// nota: aqui podemos verificar si la bala llego a una celda con tanque y aplicar daño
}

void Bullet::clearPath()
{
	delete[] this->pathToGo;
	this->pathToGo = nullptr;
	this->pathSize = 0;
	this->pathIndex = 0;

}
//gets y sets de la posicion

int Bullet::getCurrentRow() 
{
	return this->currentRow; 
}

int Bullet::getCurrentCol() 
{ 
	return this->currentCol; 
}

void Bullet::setCurrentRow(int row) 
{ 
	this->currentRow = row; 
}

void Bullet::setCurrentCol(int col) 
{ 
	this->currentCol = col; 
}
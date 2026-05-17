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

	// atributos para el rebote
	this->bounceCount = 0;
	// direccion en "x" y en "y"
	this->dirRow = 0;
	this->dirCol = 0;
	this->goalRow = 0;
	this->goalCol = 0;

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
	this->drawPath();

}

//dibuja el path de la bala
void Bullet::drawPath()
{
	//si se esta moviendo y tiene un path
	if (this->isMoving && this->pathToGo != nullptr) {
		sf::RectangleShape cellToColor(sf::Vector2f(this->cellWidth, this->cellHeight));

		cellToColor.setOutlineThickness(-1);
		cellToColor.setOutlineColor(sf::Color(100, 75, 80, 255));
		cellToColor.setFillColor(sf::Color(100, 75, 80, 60));

		for (int i = 0; i < this->pathSize; i++) {
			// pathToGo guarda indices del grafo, dividir entre MAP_SIZE da la fila
			// el modulo da la columna, si ves es igual que el que esta en map (to row y tocol)
			int row = this->pathToGo[i] / MAP_SIZE;
			int col = this->pathToGo[i] % MAP_SIZE;
			cellToColor.setPosition(col * this->cellWidth, row * this->cellHeight);
			this->window->draw(cellToColor);
		}
	}
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
}

void Bullet::setPosition(float x, float y)
{
	this->visualX = x;
	this->visualY = y;
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

// metodos para el rebote de la bala

//cuantos rebotes lleva
int Bullet::getBounceCount() 
{ 
	return this->bounceCount; 
}

void Bullet::incrementBounce() 
{ 
	this->bounceCount++; 
}

//retornan la direccion actual en filas y cols
int Bullet::getDirRow() 
{ 
	return this->dirRow; 
}

int Bullet::getDirCol() 
{ 
	return this->dirCol; 
}

//pone una direcciona a la bala (para el rebote)
void Bullet::setDir(int dirRow, int dirCol)
{
	this->dirRow = dirRow;
	this->dirCol = dirCol;
}

//retornan la fila y col objetivo inicial
int Bullet::getGoalRow() 
{ 
	return this->goalRow; 
}

int Bullet::getGoalCol() 
{ 
	return this->goalCol; 
}

// pone el objetivo nuevo del disparo (despues de un rebote)
void Bullet::setGoal(int goalRow, int goalCol)
{

	this->goalRow = goalRow;
	this->goalCol = goalCol;
}

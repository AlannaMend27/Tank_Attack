#pragma once
#include <SFML/Graphics.hpp>
#include "GameConfig.h"

class Bullet {
private:
	// ATRIBUTOS

	//pos logica
	int currentRow;
	int currentCol;

	float cellWidth;
	float cellHeight;

	//atributos de la ventana
	sf::RenderWindow* window;

	// textura y sprite de la bala
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;

	//pos visual 
	float visualX;
	float visualY;

	// path de la bala
	int* pathToGo;
	int pathSize;
	int pathIndex;
	bool isMoving;

	//metodos privados
	void initBullet();
	void renderBullet();

public:
	// constructor y destructor
	Bullet(int row, int col, sf::Vector2u windowSize, sf::RenderWindow* window);
	~Bullet();

	//metodos publicos
	void createBullet();
	void setPathToGo(int* path, int pathSize);
	int* getPathToGo();
	int GetPathSize();
	int getPathSize();
	int getPathIndex();
	void incrementPathIndex();
	bool getIsMoving();
	void setIsMoving(bool value);
	sf::Vector2f getSpritePosition();
	void moveSprite(float dx, float dy);
	void setPosition(float x, float y);
	void clearPath();
	int getCurrentRow();
	int getCurrentCol();
	void setCurrentRow(int row);
	void setCurrentCol(int col);
	// nota: aqui podes poner getTankHit, para saber si la bala toco un tanque, o un getArea() igual que tanque
	// realmente esta clase es casi identica a lo del movimiento del tanque
};
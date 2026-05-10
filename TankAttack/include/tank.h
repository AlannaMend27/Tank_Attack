#pragma once
#include <SFML/Graphics.hpp>
#include "GameConfig.h"
class Tank {
private:

	//ATRIBUTOS
	int currentRow;
	int currentCol;
	std::string tankID;

	// atributos de la ventana
	sf::Vector2u windowSize;
	sf::RenderWindow* window;
	float cellWidth;
	float cellHeight;

	std::string texturePath; //Por que son 4 texturas
	sf::Texture tankTexture;
	sf::Sprite tankSprite;

	// movimiento del tanque
	int* pathToGo;
	int pathSize;
	int pathIndex;
	bool isMoving;
	float visualX;
	float visualY;

	//metodos privados
	void initTank();
	void renderTank();

public:
	//Constructor y destructor
	Tank(int row, int col, sf::Vector2u windowSize, sf::RenderWindow* window, std::string texturePath, std::string id);
	~Tank();

	//Metodos publicos
	void createTank(); 
	int getCurrentRow();
	int getCurrentCol();
	std::string getId();

	// metodos que establecer o devuelven atribuos utiles para el movimiento del tanque
	void setPathToGo(int* path, int sisePath);
	int* getPathToGo();
	int GetPathSize();
	void incrementPathIndex();
	int getPathIndex();
	bool getIsMoving();
	void setIsMoving(bool value);
	sf::Vector2f getSpritePosition();
	void setPosition(float x, float y);
	void moveSprite(float dx, float dy);
	void setCurrentRow(int row);
	void setCurrentCol(int col);
	void clearPath();

	sf::FloatRect getArea();
};
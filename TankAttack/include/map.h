#pragma once
#include "grafo.h"
#include "queue.h"	
#include "GameConfig.h"
#include <SFML/Graphics.hpp>

class Map {
private:

	//ATRIBUTOS
	//elementos del mapa
	graph* graphMap;
	int size;

	// cambiar tamano de la matriz
	bool mapMatrix [MAP_SIZE][MAP_SIZE];

	// interfaz del mapa
	
	sf::Vector2u windowSize;
	sf::RenderWindow* window;

	sf::Texture wall;
	sf::Sprite wallMap;

	sf::Texture background;
	sf::Sprite backgroundMap;

	// metodos privados

	void generateObstacles();
	void generateGraph();
	int toIndex(int row, int col);
	int toRow(int index);
	int toCol(int index);

	bool isPositionValid(int row, int col);
	bool isCorner(int row, int col);
	bool isEveryNodeAccessible();

	void renderMap();


public:
	// constructor y destructor
	Map(int n, sf::RenderWindow* windowGame, sf::Vector2u sizeWindow);
	~Map();

	//metodos publicos
	void initMap();
	void createMap();
	void drawMap();

};
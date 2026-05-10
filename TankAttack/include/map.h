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

	// matriz del mapa
	int** mapMatrix;

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

	bool isPositionValid(int row, int col);
	bool isCellFree(int row, int col);
	const int* const* getAdjMatrix() const;
	int** getMapMatrix()const;


	// convierten indices de la matriz del juego a la matriz de adyacencia del grafo y viceversa
	int toIndex(int row, int col);
	int toRow(int index);
	int toCol(int index);



};
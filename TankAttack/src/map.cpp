#include "map.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "grafo.h"

// constructor y destructor
Map::Map(int n, sf::RenderWindow* windowGame, sf::Vector2u sizeWindow) {
	this->size = n;
	this->window = windowGame;
	this->windowSize = sizeWindow;
	initMap();
	createMap();
}

Map::~Map() {
	delete graphMap;

}

// metodos publicos

void Map::initMap()
{
	// muros
	this->wall.loadFromFile("assets/textures/wall.png");
	this->wallMap.setTexture(this->wall);

	// mapa
	this->backgruound.loadFromFile("assets/textures/backgroundMap.png");
	this->backgroundMap.setTexture(this->backgruound);


}

void Map::createMap()
{
	this->graphMap = new graph(this->size);
	this->generateObstacles();
	this->generateGraph();
	this->renderMap();
}

// metodos privados

void Map::generateObstacles()
{
	// de momento no es aleatorio, sino que inicializa con algunos obstaculos (solo para visualizar)

	// crear mapa
	for (int i = 0; i < this->mapSize; i++) {
		for (int j = 0; j < this->mapSize; j++) {
			this->mapMatrix[i][j] = 0;
		}
	}

	// crear algunos obstaculos
	for (int i = 0; i < this->mapSize; i++) {
		if (i == 4 || i == 8) {
			int cont = 8;
			while (cont>4) {
				this->mapMatrix[i][cont] = 1;
				cont--;
			}
		}
	}


}

void Map::generateGraph()
{
	// este metodo recorre las celdas de la matriz y crea cada nodo y sus conexiones 
	for (int i = 0; i < this->mapSize; i++) {
		for (int j = 0; j < this->mapSize; j++) {
			if (mapMatrix[i][j] == 0) {
				// ver si hay vecinos en los bordes
				// se revisan los vecinos sumando estos pares a la matriz (-1,0) es arriba, (1,0) es abajo,(0,-1) esizquierda ,(0,1) es derecha 
				int rows[4] = { -1,1,0,0};
				int columns[4] = { 0,0,-1,1};

				// calcular donde estarian los vecinos
				for (int k = 0; k < 4; k++) {
					int newRow = rows[k] + i;
					int newCol = columns[k] + j;

					// calcular si las posiciones son validas
					if (newRow >= 0 && newRow < this->mapSize && newCol >= 0 && newCol < this->mapSize) {
						// si la posicion es valida y la celda esta libre, agregar arista
						if (this->mapMatrix[i][j] == 0) {
							this->graphMap->createEdge(toIndex(i,j) , toIndex(newRow, newCol), 1);
							this->graphMap->createNode(1,toIndex(i,j));
						}
					}
				}
			}
		}

	}
}

int Map::toIndex(int row, int col)
{
	// convierte un indice de la matriz del mapa a un indice en la matriz de adyacencia del grafo
	return row * this->mapSize + col;

	// notita: seria bueno generar uno inverso para traducir las coordenadas de un nodo del grafo a las de la matriz de mapa

}

void Map::renderMap()
{
	// dibujar mapa de fondo
	this->backgroundMap.setScale(
		(float)this->windowSize.x / this->backgruound.getSize().x,
		(float)this->windowSize.y / this->backgruound.getSize().y
	);
	this->window->draw(this->backgroundMap);

	// obtener tamano de celdas y escalar muros a este tamano
	float cellWidth = (float)this->windowSize.x / this->mapSize;
	float cellHeight = (float)this->windowSize.y / this->mapSize;

	this->wallMap.setScale(cellWidth / this->wall.getSize().x, cellHeight / this->wall.getSize().y);

	for (int i = 0; i < this->mapSize; i++) {
		for (int j = 0; j < this->mapSize; j++) {
			if (this->mapMatrix[i][j] == 1) {
				this->wallMap.setPosition(
					j * cellWidth,
					i * cellHeight
				);
				this->window->draw(this->wallMap);
			}

		}
	}

}



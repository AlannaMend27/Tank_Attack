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
	this->background.loadFromFile("assets/textures/mapBackground.png");
	this->backgroundMap.setTexture(this->background);


}

void Map::createMap()
{
	this->graphMap = new graph(MAP_SIZE * MAP_SIZE);
	this->generateObstacles();

	//Si no todos los nodos son accesibles, genere los obstaculos de nuevo, hasta que si
	while (!this->isEveryNodeAccessible()) {
		this->generateObstacles();
	}

	this->generateGraph();
	this->renderMap();
}

// metodos privados

void Map::generateObstacles()
{

	//borramos la matriz, por si los nodos no son acessibles y se regeneran los obstaculos. es decir creamos un nuevo mapa aleatorio
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			this->mapMatrix[i][j] = 0;
		}
	}

	//Numero de muros colocados por el momento
	int wallsPlaced = 0;

	while (wallsPlaced < TOTAL_WALLS) {

		// se colocan los muros aleatoriamente
		int randomRow = rand() % MAP_SIZE;
		int randomCol = rand() % MAP_SIZE;

		// solo coloca el muro si no es una esquina y si esta libre, si ya hay un muro ahi intenta otra posicion
		if (!this->isCorner(randomRow, randomCol) && this->mapMatrix[randomRow][randomCol] == 0) {

			//se coloca el muro y lo marcamos
			this->mapMatrix[randomRow][randomCol] = 1;
			wallsPlaced++;
		}

	}

}

void Map::generateGraph()
{
	// este metodo recorre las celdas de la matriz y crea cada nodo y sus conexiones 
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
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
					if (this->isPositionValid(newRow, newCol)) {
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
	return row * MAP_SIZE + col;

	// notita: ya esta el inverso :D

}

// convierten un indice de la matriz de adyacencia del grafo a un indice de la matriz del mapa

int Map::toRow(int index)
{
	return index / MAP_SIZE;
}

int Map::toCol(int index) 
{
	return index % MAP_SIZE;
}


//Metodos que verifican ciertos parametros de las posiciones

bool Map::isPositionValid(int row, int col) 
{
	//Verifica si la la fila/col no es negativa y si no es mayor al tamanio del mapa
	if (row >= 0 && row < MAP_SIZE && col >= 0 && col < MAP_SIZE) {
		return true;
	}
	else {
		return false;
	}
}


//Para proteger los nodos de las esquinas (para los tanques al inicio)

bool Map::isCorner(int row, int col) 
{
	if ((row == 0 && col == 0) || (row == 0 && col == MAP_SIZE - 1) || (row == MAP_SIZE -1  && col == 0) || (row == MAP_SIZE - 1 && col == MAP_SIZE - 1)) {
		return true;
	}
	else {
		return false;
	}
}

bool Map::isEveryNodeAccessible() 
{
	//El "BFS" implementado aqui es solo de verificacion, el de los tanques retorna la posicion, ese es diferente

	//Creamos una matriz empezada todo en falso, es para decir si ya fuimos a esa posicion, la cola es para los que faltan por visitar
	bool visited[MAP_SIZE][MAP_SIZE] = {};
	Queue BFSQueue;

	//Se arranca a visitar desde la esquina superior izquiera (0,0) y se marca como visitada
	visited[0][0] = true;
	BFSQueue.enqueue(this->toIndex(0, 0)); //se aniade a la cola

	int Rows[4] = { -1,1,0,0 }; // mismos pares (-1,0) up, (1,0) down , (0,-1) left y (0,1) right
	int Cols[4] = { 0,0,-1,1 };

	/*
	El flujo de este while es el siguiente:
	- Mientras la cola no este vacia, saca la primera posicion en la cola 
	- Convierte esa posicion convierte a un indice de la matriz (es el inverso de toindex)
	- Luego de tener las posiciones vamos a las 4 posiciones las de los arreglos Rows y Cols
	- Mientras se recorre el for hay un if que verifica lo siguiente
		1. isPositionValid() -> Si no se sale del mapa o bien si la posicion no es negativa
		2. !visited sis no lo heos visitado aun
		3. mapMatrix == 0 es una celda libre no muro
	- En caso de que todo lo anterior sea true, agregamos la posicion que se puede visitar 
	- Hacemos lo mismo hasta que la cola este vacia
	*/
	while (!BFSQueue.isEmpty()) {

		int currentCell = BFSQueue.peek();
		BFSQueue.dequeue();

		//convertir el indice a fila y columna
		int currentRow = this->toRow(currentCell);
		int currentCol = this->toCol(currentCell);


		for (int i = 0; i < 4; i++) {

			int neighborRow = currentRow + Rows[i];
			int neighborCol = currentCol + Cols[i];

			if (this->isPositionValid(neighborRow, neighborCol) && !visited[neighborRow][neighborCol] && this->mapMatrix[neighborRow][neighborCol] == 0) {
				visited[neighborRow][neighborCol] = true;
				BFSQueue.enqueue(this->toIndex(neighborRow, neighborCol));
			}
		}
	}

	/*
	Esto es la verificacion definitiva, recorre la matriz buscando celdas libres == 0 
	si alguna de esas no fue visitada (!visited) es por que hay una celda sola o aislada,
	retorna false si e el caso, si no true, esto por que a la hora de crear el mapa
	si este metodo es falso se generan muros aleatorios hasta que sea true
	*/
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (this->mapMatrix[i][j] == 0 && !visited[i][j]) {
				return false;
			}
		}
	}

	return true;
}

void Map::renderMap()
{
	// dibujar mapa de fondo
	this->backgroundMap.setScale(
		(float)this->windowSize.x / this->background.getSize().x,
		(float)this->windowSize.y / this->background.getSize().y
	);
	this->window->draw(this->backgroundMap);

	// obtener tamano de celdas y escalar muros a este tamano
	float cellWidth = (float)this->windowSize.x / MAP_SIZE;
	float cellHeight = (float)this->windowSize.y / MAP_SIZE;

	this->wallMap.setScale(cellWidth / this->wall.getSize().x, cellHeight / this->wall.getSize().y);

	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
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

//Este metodo sirve para render game, si la celda es libre, se puede mover/disparar con el tanque ahi
bool Map::isCellFree(int row, int col)
{
	return this->mapMatrix[row][col] == 0;
}

//Publico para que pueda ser llamado
void Map::drawMap() {

	this->renderMap();

}

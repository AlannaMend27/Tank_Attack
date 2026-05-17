#pragma once
#include "grafo.h"
#include "GameConfig.h"

class AStar {
private: 
	//ATRIBUTOS
	const int* const* graphMatrix;
	int startIndex;
	int goalIndex;

	// variables de A*
	int* path;
	bool visited[GRAPH_SIZE];
	int previous[GRAPH_SIZE];
	int pathSize;

	// costo (el g osea Weight) desde el inicio hasta cada nodo
	int gCost[GRAPH_SIZE];

	// costo total (f+g) con todo y heuristica
	int fCost[GRAPH_SIZE];

	// control de la cola de prioridad y cuantos elementos hay en ella
	int priorityQueue[GRAPH_SIZE];
	int contQueue;

	//Metodos privados
	int minorElement();
	int* reconstructPath();

	
	// con manhattan
	int heuristic(int index);

public: 

	//constructor y destructor
	AStar(const int* const* matrix);
	~AStar();

	//metodos publicos
	int* AStarAlgorithm(int startIndex, int goalIndex);
	int getPathSize();
};
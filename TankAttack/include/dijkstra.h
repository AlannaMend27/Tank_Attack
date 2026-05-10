#pragma once
#include "grafo.h"
#include "GameConfig.h"


class Dijkstra {
private:
	// ATRIBUTOS
	const int* const* graphMatrix;
	int startIndex;
	int goalIndex;

	// variables del algoritmo
	int* path;
	bool visited[GRAPH_SIZE];
	int previous[GRAPH_SIZE];
	int pathSize;
	int distance[GRAPH_SIZE];

	// control de la cola de prioridad y cuantos elementos hay en ella
	int priorityQueue[GRAPH_SIZE];
	int contQueue;

	// metodos privados
	int MinorElement();
	int* shortestPath();

public:

	// constructor y destructor
	Dijkstra(const int* const* matrix);
	~Dijkstra();

	// metodos publicos
	int* DijkstraAlgorithm(int startIndex, int goalIndex);
	int getPathSize();

};
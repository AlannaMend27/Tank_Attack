#pragma once
#include "grafo.h"
#include "GameConfig.h"


class Dijkstra {
private:
	// ATRIBUTOS

	// estos son los atributos que recibe el algoritmo para funcionar
	const int* const* graphMatrix;
	int startIndex;
	int goalIndex;

	// variables del algoritmo
	int* path;
	bool visited[GRAPH_SIZE];
	int previous[GRAPH_SIZE];
	int pathSize;

	// lleva la cuenta del tamamio del camino recorrido en el momento
	int distance[GRAPH_SIZE];

	// estos atributos llevan control de la cola de prioridad y cuantos elementos hay en ella
	int priorityQueue[GRAPH_SIZE];
	int contQueue;


	int MinorElement();
	int* shortestPath();

public:

	// constructor y destructor
	Dijkstra(const int* const* matrix);
	~Dijkstra();

	// algoritmo de dijkstra
	int* DijkstraAlgorithm(int startIndex, int goalIndex);

	// retorna el tamanio del camino encontrado por dijkstra
	int getPathSize();

};
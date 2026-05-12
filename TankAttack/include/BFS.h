#pragma once
#include "grafo.h"
#include "GameConfig.h"
#include "queue.h"

class BFS {
private:
	//ATRIBUTOS
	const int* const* graphMatrix;

	//variables del algoritmo
	int* BFSpath;
	int pathSize;
	int startIndex;
	int goalIndex;
	bool visited[GRAPH_SIZE];
	int previous[GRAPH_SIZE];

public:
	//constructor y destructor 
	BFS(const int* const* matrix);
	~BFS();

	//Metodos publicos
	int* rebuildPath();
	int* BFSAlgorithm(int startIndex, int goalIndex);
	int getPathSize();
	void clearBFS();

};
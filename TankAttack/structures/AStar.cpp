#include "AStar.h"

//constructor y destructor
AStar::AStar(const int* const* matrix)
{
	this->graphMatrix = matrix;
	this->path = nullptr;
	this->pathSize = 0;
	this->startIndex = 0;
	this->goalIndex = 0;
	this->contQueue = 0;

	for (int i = 0; i < GRAPH_SIZE; i++) {
		this->gCost[i] = INT_MAX;
		this->fCost[i] = INT_MAX;
		this->visited[i] = false;
		this->previous[i] = -1;
		this->priorityQueue[i] = -1;
	}
}

AStar::~AStar()
{
	delete[] this->path;
}

//METODOS PRIVADOS

//este metodo calcula la distancia manhattan, convierte los indices del grafo a la matriz

int AStar::heuristic(int index)
{

	int currentRow = index / MAP_SIZE;
	int currentCol = index % MAP_SIZE;
	int rowGoal = this->goalIndex / MAP_SIZE;
	int colGoal = this->goalIndex % MAP_SIZE;
	
	//la misma formula de la calcu
	int manhattanDistance = abs(currentRow - rowGoal) + abs(currentCol - colGoal);
	return manhattanDistance;

}

// retorna el nodo con menor fcost, es lo mismo que minorelement de dijsktra pero compara fCost en vez de distance

int AStar::minorElement() 
{
	int minor = -1;
	for (int i = 0; i < GRAPH_SIZE; i++) {
		if (!this->visited[i] && this->fCost[i] != INT_MAX) {
			if (minor == -1 || this->fCost[i] < fCost[minor]) {
				minor = i;
			}
		}
	}
	return minor;
}

//igual que shortestpath de dijktra, reconstruye el camino desde el obj hasta el inicio
int* AStar::reconstructPath()
{
	int father = this->goalIndex;
	while (father != -1) {
		this->pathSize++;
		father = this->previous[father];
	}

	this->path = new int[this->pathSize];
	father = this->goalIndex;
	for (int i = this->pathSize - 1; i >= 0; i--) {
		this->path[i] = father;
		if (father != -1) {
			father = this->previous[father];
		}

	}

	return this->path;
}

// METODOS PUBLICOS

// ejecuta A* desde el indice de inicio hasta el goal, la diferencia con dijkstra es que usa f+g (costo y heuristica)

int* AStar::AStarAlgorithm(int startIndex, int goalIndex)
{
	this->startIndex = startIndex;
	this->goalIndex = goalIndex;
	//iniciar el nodo del inicio en 0
	this->gCost[startIndex] = 0;
	this->fCost[startIndex] = this->heuristic(startIndex);
	this->priorityQueue[this->startIndex] = this->fCost[this->startIndex];
	this->contQueue++;

	while (this->contQueue != 0) {

		// encontrar el nodo con menor f (costo) y marcarlo como visitado
		int current = this->minorElement();

		//si no hay nodos disponibles, no se puede legar al objetivo 
		if (current == -1) {
			break;
		}

		this->visited[current] = true;
		this->contQueue--;

		//si llegamos al objetivo paramos
		if (current == this->goalIndex) {
			break;
		}

		//revisar los vecinos del nodo actual
		for (int i = 0; i < GRAPH_SIZE; i++) {
			if (this->graphMatrix[current][i] != 0 && !this->visited[i]) {
				int newGCcost = this->gCost[current] + 1;

				if (this->gCost[current] != INT_MAX && newGCcost < this->gCost[i]) {
					this->previous[i] = current;
					this->gCost[i] = newGCcost;
					// diferencia con dijkstra, fCost tiene manhattan
					this->fCost[i] = newGCcost + this->heuristic(i);
					this->priorityQueue[i] = this->fCost[i];
					this->contQueue++;
				}
			}
		}
	}
	return this->reconstructPath();
}

int AStar::getPathSize()
{
	return this->pathSize;
}
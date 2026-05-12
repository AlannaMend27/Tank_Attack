#include "BFS.h"

// constructor y destructor

BFS::BFS(const int* const* matrix)
{
	this->graphMatrix = matrix;
	this->BFSpath = nullptr;
	this->pathSize = 0;
	this->startIndex = 0;
	this->goalIndex = 0;

	for (int i = 0; i < GRAPH_SIZE; i++) {
		this->visited[i] = false;
		this->previous[i] = -1;
	}
}

BFS::~BFS() 
{
	delete[] BFSpath;
}

//Este metodo es identico a shortestPath de Dijkstra
int* BFS::rebuildPath() 
{
	int father = this->goalIndex;

	while (father != -1) {
		this->pathSize++;
		father = previous[father];
	}

	this->BFSpath = new int[this->pathSize];
	father = this->goalIndex;
	//Hacemos el camino de atras para adelante
	for (int i = this->pathSize - 1; i >= 0; i--) {
		this->BFSpath[i] = father;
		if (father != -1) {
			father = this->previous[father];
		}
	}

	return BFSpath;

}

//EL codigo es muy similar a isEveryNodeAccesible, solo que sin la verificacion final
int* BFS::BFSAlgorithm(int startIndex, int goalIndex)
{
	this->startIndex = startIndex;
	this->goalIndex = goalIndex;

	//Marcamos el nodo inicial como visitado
	this->visited[startIndex] = true;

	Queue BFSQueue;
	//Iniciamos la cola con el nodo inicial
	BFSQueue.enqueue(startIndex);

	while (!BFSQueue.isEmpty()) {

		int currentNode = BFSQueue.peek();
		BFSQueue.dequeue();

		//Si se llega al objetivo parar
		if (currentNode ==goalIndex) {
			break;
		}

		// revisar los vecinos del nodo actual 
		for (int i = 0; i < GRAPH_SIZE; i++) {

			//si hay conexion y no ha sido visitado el vecino, guardamos de donde venimos y lo metemos a la cola
			if (this->graphMatrix[currentNode][i] != 0 && !this->visited[i]) {
				this->visited[i] = true;
				this->previous[i] = currentNode;
				BFSQueue.enqueue(i);
			}

		}
	}

	return this->rebuildPath();
	


}

int BFS::getPathSize() 
{
	return this->pathSize;
}

//Para reutilizar el objeto
void BFS::clearBFS() 
{
	delete[] this->BFSpath;
	this->BFSpath = nullptr;
	this->pathSize = 0;
	this->startIndex = 0;
	this->goalIndex = 0;

	for (int i = 0; i < GRAPH_SIZE; i++) {
		this->visited[i] = false;
		this->previous[i] = -1;
	}

}
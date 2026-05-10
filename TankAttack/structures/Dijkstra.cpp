#include "dijkstra.h"


// constructor y destructor
Dijkstra::Dijkstra(const int* const* matrix) {
	this->graphMatrix = matrix;
	this->path = nullptr;
	this->contQueue = 0;
	this->pathSize = 0;
	this->goalIndex = 0;
	this->startIndex = 0;

	for (int i = 0; i < GRAPH_SIZE; i++) {
		this->distance[i] = INT_MAX;
		this->visited[i] = false;
		this->previous[i] = -1;
		this->priorityQueue[i] = -1;
	}

}

Dijkstra::~Dijkstra() {
	delete[] path;
}

// METODOS PRIVADOS

int Dijkstra::MinorElement()
{
	int menor = -1; // empezamos sin ningún candidato

	for (int i = 0; i < GRAPH_SIZE; i++) {
		// solo considerar nodos no visitados y que estén en la cola (los que tengas un peso infinito INT MAX)
		if (!this->visited[i] && this->distance[i] != INT_MAX) {
			// si no hay candidato aún o encontramos uno nodo con menor peso entonces actualizamos la variable menor
			if (menor == -1 || this->distance[i] < this->distance[menor]) {
				menor = i;
			}
		}
	}

	return menor;
}

// este metodo recorre el camino para llegar del nodo destino al nodo inicial, es la parte final del algoritmo
int* Dijkstra::shortestPath()
{
	// calcular cuentos nodos hay que recorrer antes de llegar al destino
	int father = this->goalIndex;
	// hasta que lleguemos al inicio que sería un nodo que no tenga padre asignado
	while (father != -1) {
		this->pathSize++;
		father = previous[father];

	}

	this->path = new int[this->pathSize];
	father = this->goalIndex;
	// generar el camino (de nodo actual al destinol)
	for (int i = this->pathSize-1; i >= 0 ;i--) {
		this->path[i] = father;
		if (father != -1) {
			father = this->previous[father];
		}

	}
	// retorna el camino desde el nodo inicial para llegar al destino
	return path;

}

// METODOS PUBLICOS

int* Dijkstra::DijkstraAlgorithm(int startIndex, int goalIndex)
{
	this->startIndex = startIndex;
	this->goalIndex = goalIndex;
	this->distance[this->startIndex] = 0;
	priorityQueue[this->startIndex] = 0;
	this->contQueue++;

	// funcionar hasta que no queden más elementos en la cola
	while (this->contQueue != 0) {

		// encontrar elemento más pequeño en la cola de prioridad y marcarlo como visitado
		int min = this->MinorElement();
		this->visited[min] = true;
		this->contQueue--;


		// revisar si hemos llehado al destino
		if (min == this->goalIndex) {
			break;
		}

		//revisar los vecinos que desbloquea el nodo visitado
		for (int i = 0; i < GRAPH_SIZE; i++) {
			// verificar que exista una arista entre el nodo visitado y el nodo vecino, 
			// y que el nodo vecino no haya sido visitado
			if (this->graphMatrix[min][i] != 0 && !this->visited[i]) {
				// verificar si el nodo actual es alcanzable y si hay un camino mas corto al actual para ese nodo (inician en infinito)
				// notita: ese 1 hay que cambiarlo, solo es ese pq ahorita todos los nodos tienen un pesito de 1
				if (this->distance[min] != INT_MAX && this->distance[min] + 1 < this->distance[i]) {
					// asignar el peso que tiene llegar a ese vecino
					this->distance[i] = this->distance[min] + 1;
					// guardar el camino de donde venimos
					this->previous[i] = min;
					// agregarlo a la cola para procesarlo despues
					this->priorityQueue[i] = this->distance[i];
					this->contQueue++;

				}

			}
		}


	}

	// reconstruir el camino para retornarlo luego
	return this->shortestPath();
}

// retorna el tamanio del camino mas corto
int Dijkstra::getPathSize()
{
	return this->pathSize;
}

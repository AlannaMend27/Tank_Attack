#include "grafo.h"
#include "GameConfig.h"
// destructor y constructor clase nodo
Node::Node(int weigthNode)
{
	this->weight = weigthNode;
}


// destructor y constructor clase grafo

graph::graph(int n)
{
	this->size = n;
	this->capacity = n * n;
	this->matrix = new int*[this->size];

	for (int i = 0; i < this->size; i++) {
		this->matrix[i] = new int[this->size];
		for (int j = 0; j < this->size; j++) {
			this->matrix[i][j] = 0;
		}
	}
}

graph::~graph()
{
	for (int i = 0; i < this->size; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

// metodos publicos de la clase

void graph::createEdge(int a, int b, int weightNode)
{
	if (a < this->size && b < this->size) {
		matrix[a][b] = weightNode;
		matrix[b][a] = weightNode;
	}

}

void graph::createNode(int weightNode, int id)
{
	if (this->Nodes.size() < capacity) {
		Node newNode = Node(weightNode);
		this->Nodes.insert({id,newNode});
	}
}

//mopdifica una arista en la matriz de adyacencia (por si hay que actualizar que hay un tanque ahi)
//nodoA es el origen, B el destino, value es 1 si hay coexion 0 si no
void graph::setEdge(int nodeA, int nodeB, int value) 
{
	if (nodeA < this->size && nodeB < this->size) {
		//Ponemos la conexion en digamos "true o false" y como es no dirigido en ambas direcciones
		this->matrix[nodeA][nodeB] = value;
		this->matrix[nodeB][nodeA] = value;
	}
}

const int* const* graph::getMatriz() const
{
	return this->matrix;
}



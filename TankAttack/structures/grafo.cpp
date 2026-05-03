#include "grafo.h"

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

// metodos públicos de la clase

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

const int* const* graph::getMatriz() const
{
	return this->matrix;
}



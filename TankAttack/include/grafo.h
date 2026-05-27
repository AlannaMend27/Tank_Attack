#pragma once
#include<unordered_map>

class Node {
public:
	int weight;

	// constructor
	Node();
	Node(int weightNode);
};

class graph {
private:
	// ATRIBUTOS
	int size;
	int capacity;
	int** matrix;

public:
	// constructor y destructor del grafo
	graph(int n);
	~graph();

	// metodos para crear la matriz
	void createEdge(int a, int b, int weightNode);
	const int* const* getMatriz() const;

	void setEdge(int nodeA, int nodeB, int value);



};


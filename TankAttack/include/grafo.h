#pragma once
#include<unordered_map>

class Node {
public:
	int weight;

	// constructor
	Node(int weigthNode);
};

class graph {
private:
	// ATRIBUTOS
	int size;
	int capacity;
	int** matrix;
	std::unordered_map<int, Node> Nodes;

public:
	// constructor y destructor del grafo
	graph(int n);
	~graph();

	// metodos para crear la matriz
	void createEdge(int a, int b, int weightNode);
	void createNode(int weightNode, int id);
	const int* const* getMatriz() const;

	void setEdge(int nodeA, int nodeB, int value);



};



#pragma once
#include <iostream>

class QueueNode{
public:
    QueueNode* next;
    int value;

    //Constructor
    QueueNode(int value);

};

class Queue {
private:

    //ATRIBUTOS
    int size;
    QueueNode* first;
    QueueNode* last;

public:
    // constructor y destructor de la cola
    Queue();
    ~Queue();

    //Metodos
    void enqueue(int value);
    int dequeue();
    int peek();
    int getSize();
    int peekAt(int index);
    bool isEmpty();


    QueueNode* getFirst();

};
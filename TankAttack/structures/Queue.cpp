#include "Queue.h"

//Constructor del nodo
QueueNode::QueueNode(int value)
{
    this->value = value;
    this->next = nullptr;
}

//Constructor y destructor de la cola, el del nodo lo borra el mismo de la cola
Queue::Queue()
{
    this->size = 0;
    this->first = nullptr;
    this->last = nullptr;
}

Queue::~Queue()
{

    while (!this->isEmpty()) {
        this->dequeue();
    }

}

// metodos privados de la clase

void Queue::enqueue(int value)
{
    QueueNode* nodeToEnqueue = new QueueNode(value);
    if (this->first == nullptr) {
        this->first = nodeToEnqueue;
        this->last = nodeToEnqueue;
        this->size++;  
        return;
    }
    this->last->next = nodeToEnqueue;
    this->last = nodeToEnqueue;
    this->size++;
}

int Queue::dequeue()
{
    if (this->first == nullptr) {
        return -1;
    }
    QueueNode* temporaryNode = this->first;
    int value = temporaryNode->value;
    this->first = this->first->next;

    if (this->first == nullptr) {
        this->last = nullptr;
    }
    delete temporaryNode;
    this->size--;
    return value;
}

int Queue::peek()
{

    return this->first->value;
}

bool Queue::isEmpty()
{
    return this->first == nullptr;
}

int Queue::getSize()
{
    return this->size;
}


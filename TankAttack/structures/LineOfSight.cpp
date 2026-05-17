#include "lineOfSight.h"
#include < cmath >

// constructor y destructor
LineOfSight::LineOfSight(int** matrix)
{
	this->dx = 0;
	this->dy = 0;
	this->error = 0;
	this->row = 0;
	this->col = 0;
	this->increment1 = 0;
	this->increment2 = 0;
	this->incrementX = 1;
	this->incrementY=1;
	this->mapMatrix = matrix;
	this->path = nullptr;
	this->pathSize = 0;

}

LineOfSight::~LineOfSight()
{
	delete[] path;
}

// METODOS PUBLICOS
bool LineOfSight::LineOfSightAlgorithm(int rowStart, int colStart, int rowGoal, int colGoal)
{
	this->row = rowStart;
	this->col = colStart;

	// calcular distancia absoluta entre ambas direcciones
	this->dy = abs(rowGoal - this->row);
	this->dx = abs(colGoal - this->col);

	// calcular la direccion del movimiento
	if (rowGoal < this->row) {
		// bajar en x
		this->incrementY = -1;
	}

	if (colGoal < this->col) {
		// subir en y
		this->incrementX = -1;
	}

	// si la linea es mas horizontal (avanza mas en X que en Y)
	if (this->dx > this->dy) {

		this->path = new int[this->dx];

		// calcular el error (que tan lejos estamos de la linea recta ideal)
		this->error = 2 * this->dy - this->dx;
		this->increment1 = 2 * (this->dy - this->dx);
		this->increment2 = 2 * this->dy;

		for (int i = 0; i < this->dx; i++) {
			// si el error acumulado supero el umbral, nos movemos en el eje y (filitas)
			if (this->error >= 0) {
				this->row += this->incrementY;
				this->error += increment1; 
			}

			else {
				// el error es bajo, no nos movemos en eje y
				this->error += increment2;
			}
			// se avanza en eje x
			this->col += this->incrementX;

			// verificar si la celda es una pared
			if (this->mapMatrix[this->row][this->col] != 0) {
				return false;
			}
			// si no hemos chocado con una pared, la guardamos en path
			this->path[i] = this->row * MAP_SIZE + this->col;
			this->pathSize++;
		}
	}
	// si la linea es mas "vertical" (avanza mas en Y que en X)
	else {
		this->path = new int[this->dy];

		// caso en el que la diferencia en y sea mas grande que la de x
		this->error = 2 * this->dx - this->dy;
		this->increment1 = 2 * (this->dx - this->dy);
		this->increment2 = 2 * this->dx;

		for (int i = 0; i < this->dy; i++) {
			// si el error acumulado supero el umbral, nos movemos en el eje x (columnitas)
			if (this->error >= 0) {
				this->col += this->incrementX;
				this->error += increment1;
			}
			else {
				// el error es bajo, no nos movemos en eje x
				this->error += increment2;
			}
			this->row += this->incrementY;

			// verificar si la celda es una pared
			if (this->mapMatrix[row][col] != 0) {
				return false;
			}
			// si no hemos chocado con una pared, la guardamos en path
			this->path[i] = this->row * MAP_SIZE + this->col;
			this->pathSize++;
		}
	}

	return true;
}

int LineOfSight::getPathSize()
{
	return this->pathSize;
}


int* LineOfSight::getPath()
{
	return this->path;
}


int LineOfSight::getLastRow() 
{
	return this->row;
}

int LineOfSight::getLastCol() 
{
	return this->col;
}


void LineOfSight::clearLineOfSight()
{
	this->row = 0;
	this->col = 0;
	this->dx = 0;
	this->dy = 0;

	// limpiar error acumulado e incrementos
	this->error = 0;
	this->increment1 = 0;
	this->increment2 = 0;

	// limpiar incrementos de direccion
	this->incrementX = 1;
	this->incrementY = 1;

	// limpiar el path y su tamanio
	this->pathSize = 0;
}



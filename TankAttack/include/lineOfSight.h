#pragma once
#include "GameConfig.h"
#include <iostream>

class LineOfSight {
private:
	// ATRIBUTOS 
	int dx;
	int dy;
	int	row;
	int col;
	int** mapMatrix;

	// error y calculo de error
	int error;
	int increment1;
	int increment2;

	// incremento para creacion del path
	int incrementX;
	int incrementY;
	int* path;
	int pathSize;

	

public:
	// constructor y destructor
	LineOfSight(int** matrix);
	~LineOfSight();

	// metodos publicos
	bool LineOfSightAlgorithm(int rowStart, int colStart, int rowGoal, int colGoal);
	int getPathSize();
	int* getPath();
	void clearLineOfSight();
};
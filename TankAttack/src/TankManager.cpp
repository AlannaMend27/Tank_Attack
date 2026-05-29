#include "TankManager.h"

// constructor
TankManager::TankManager(Map* map, Tank** tanks, Player** players, sf::Vector2u windowSize, float cellWidth, float cellHeight)
{
    this->gameMap = map;
    this->tanks = tanks;
    this->players = players;
    this->windowSize = windowSize;
    this->cellWidth = cellWidth;
    this->cellHeight = cellHeight;

    // inicializar punteros en nullptr
    this->activeTank = nullptr;
    this->AlgDijkstra = nullptr;
    this->AlgBFS = nullptr;
    this->AlgLineOfSight = nullptr;
}

// destructor
TankManager::~TankManager()
{
    // los algoritmos se liberan despues de cada uso
    // activeTank es solo una referencia, no se libera aqui
    delete this->AlgDijkstra;
    delete this->AlgBFS;
    delete this->AlgLineOfSight;
}


// verifica si hay un tanque animandose
bool TankManager::isAnimating()
{
    return this->activeTank != nullptr && this->activeTank->getIsMoving();
}

// verifica si el tanque termino de moverse y hay que cambiar turno
bool TankManager::isReadyToSwitch()
{
    return this->activeTank != nullptr && !this->activeTank->getIsMoving();
}

// limpia el tanque activo
void TankManager::clearActiveTank()
{
    this->activeTank = nullptr;
}

// getter del tanque activo
Tank* TankManager::getActiveTank()
{
    return this->activeTank;
}

// le da el path a seguir al tanque por el algoritmo de Dijkstra
void TankManager::SetDijkstraPath(int currentIndex, int goalIndex)
{
    int* path;
    int sizeOfPath;

    // llamar a dijkstra, obtener el camino y el tamanio del camino
    this->AlgDijkstra = new Dijkstra(this->gameMap->getAdjMatrix());
    path = this->AlgDijkstra->DijkstraAlgorithm(currentIndex, goalIndex);
    sizeOfPath = this->AlgDijkstra->getPathSize();

    // si el camino es inalcanzable
    if (path == nullptr || sizeOfPath == 0) {
        return;
    }

    // establecer la ruta a seguir del tanque
    this->activeTank->setPathToGo(path, sizeOfPath);
}

// le da el path a seguir al tanque por el algoritmo BFS
void TankManager::SetBFSPath(int currentIndex, int goalIndex)
{
    int* path;
    int pathSize;

    //llamamos al algoritmo BFS, obtenemos el path y el tamanio del path
    this->AlgBFS = new BFS(this->gameMap->getAdjMatrix());
    path = this->AlgBFS->BFSAlgorithm(currentIndex, goalIndex);
    pathSize = this->AlgBFS->getPathSize();

    //camino inalcanzable
    if (path == nullptr || pathSize == 0) {
        return;
    }

    //establecer la ruta a seguir del tanque
    this->activeTank->setPathToGo(path, pathSize);
}

// le da el path a seguir al tanque por el algoritmo linea vista y llama a movimiento aleatorio en caso de ser necesario
void TankManager::SetLineOfSightPath(int currentIndex, int goalIndex)
{
    this->AlgLineOfSight = new LineOfSight(this->gameMap->getMapMatrix());
    // variables para guardar temporalmente el path
    int* path;
    int sizeOfPath;

    // obtener coordenadas actuales ( en cada iteracion por movimiento aleatorio)
    int tankRow = this->activeTank->getCurrentRow();
    int tankCol = this->activeTank->getCurrentCol();
    int goalRow = this->gameMap->toRow(goalIndex);
    int goalCol = this->gameMap->toCol(goalIndex);

    // realizar linea vista si no hay obstaculos intermedios
    if (this->AlgLineOfSight->LineOfSightAlgorithm(tankRow, tankCol, goalRow, goalCol)) {
        path = this->AlgLineOfSight->getPath();
        sizeOfPath = this->AlgLineOfSight->getPathSize();
    }
    else {
        // preparar filas y columnas para movimiento random
        int randomRow;
        int randomCol;

        // realizar un movimiento aleatorio
        this->randomMove(randomRow, randomCol, goalRow, goalCol);

        // mover el tanque a la posicion aleatoria (aparece de pronto)
        this->activeTank->setCurrentRow(randomRow);
        this->activeTank->setCurrentCol(randomCol);
        this->activeTank->setPosition(randomCol * this->cellWidth, randomRow * this->cellHeight);

        // volver a obtener las coordenadas actuales del tanque ( en cada iteracion por movimiento aleatorio)
        tankRow = this->activeTank->getCurrentRow();
        tankCol = this->activeTank->getCurrentCol();

        // volver a aplicar linea vista
        this->AlgLineOfSight->clearLineOfSight();
        this->AlgLineOfSight->LineOfSightAlgorithm(randomRow, randomCol, goalRow, goalCol);

        // utilizar el path obtenido, ya sea completo o parcial
        path = this->AlgLineOfSight->getPath();
        sizeOfPath = this->AlgLineOfSight->getPathSize();

    }

    // verificar si el camino es inalcanzable, sino no hay cambio
    if (path == nullptr || sizeOfPath == 0) {
        return;
    }

    // establecer camino que debe de seguir el tanque
    this->activeTank->setPathToGo(path, sizeOfPath);
}

void TankManager::randomMove(int& randomRow, int& randomCol, int goalRow, int goalCol)
{
    bool isfree = false;

    while (!isfree) {
        // generar una columna y una fila random
        randomRow = this->activeTank->getCurrentRow() + (rand() % (2 * RANDOM_MOVE_RADIUS + 1)) - RANDOM_MOVE_RADIUS;
        randomCol = this->activeTank->getCurrentCol() + (rand() % (2 * RANDOM_MOVE_RADIUS + 1)) - RANDOM_MOVE_RADIUS;

        // verificar que no es la posicion anterior (no hubo movimiento aleatorio)
        if (randomRow == this->activeTank->getCurrentRow() && randomCol == this->activeTank->getCurrentCol()) {
            continue;
        }

        // verificar que no es la posicion destino (llegue al destino pero no por linea vista)
        if (randomRow == goalRow && randomCol == goalCol) {
            continue;
        }

        // verificar que la celda no sea una pared y que este dentro d elos limites del mapa
        if (randomRow < MAP_SIZE && randomCol < MAP_SIZE &&
            randomRow >= 0 && randomCol >= 0 &&
            this->gameMap->isCellFree(randomRow, randomCol)) {
            isfree = true;
        }
    }
}


// Bloquea los nodos de los tanques (para detectarlos como obstaculos), recibe el tanque que no se bloquea
void TankManager::blockOtherTanks(Tank* tankToExclude)
{
    for (int i = 0; i < 4; i++) {
        if (this->tanks[i] != tankToExclude) {
            int otherTanksRow = this->tanks[i]->getCurrentRow();
            int otherTanksCol = this->tanks[i]->getCurrentCol();
            //bloqueamos los nodos donde estan los otros tanques
            this->gameMap->blockNode(this->gameMap->toIndex(otherTanksRow, otherTanksCol));
            this->gameMap->blockMapNode(otherTanksRow, otherTanksCol);
        }
    }
}

//Restaura los nodos bloqueados por el metodo anterior, esto se usa despues de calcular el path
void TankManager::unblockOtherTanks(Tank* tankToExclude)
{
    for (int i = 0; i < 4; i++) {
        if (this->tanks[i] != tankToExclude) {
            int otherTanksRow = this->tanks[i]->getCurrentRow();
            int otherTanksCol = this->tanks[i]->getCurrentCol();
            //desbloqueamos los nodos donde estan los otros tanques
            this->gameMap->unblockNode(this->gameMap->toIndex(otherTanksRow, otherTanksCol));
            this->gameMap->unblockMapNode(otherTanksRow, otherTanksCol);
        }
    }
}


void TankManager::selectPathAlgorithm(int currentIndex, int goalIndex, int currentPlayer)
{
    std::string colorTank = this->activeTank->getId();

    // si tiene el power up movementPrecision activo, forzamos bfs o dijkstra segun el color
    if (this->players[currentPlayer]->getActivePowerUp() == (int)PowerUpType::movementPrecision) {
        if (colorTank == "amarillo" || colorTank == "rojo") {
            this->SetDijkstraPath(currentIndex, goalIndex);
        }
        else {
            this->SetBFSPath(currentIndex, goalIndex);
        }
        //aplicamos el poder (lo quitamos de la cola)
        this->players[currentPlayer]->clearActivePowerUp();
        return;
    }

    if (colorTank == "amarillo" || colorTank == "rojo") {
        // Dijkstra 80% de probabilidad, Linea vista 20%
        int randomNum = rand() % 100;
        if (randomNum < 80) {
            // llamar a dijkstra y establecer el camino que debe de seguir el tanque activo
            this->SetDijkstraPath(currentIndex, goalIndex);
        }
        else {
            // llamar a linea vista y establecer el camino que debe de seguir el tanque activo
            this->SetLineOfSightPath(currentIndex, goalIndex);
        }
    }
    else {
        // BFS 50% de probabilidad, Linea vista 50%
        int randomNum = rand() % 100;
        if (randomNum < 50) {
            this->SetBFSPath(currentIndex, goalIndex);
        }
        else {
            // llamar a linea vista y establecer el camino que debe de seguir el tanque activo
            this->SetLineOfSightPath(currentIndex, goalIndex);
        }
    }
}

//Pasa el click delmouse a coordenadas de la matriz
void TankManager::mouseClickToCoords(sf::Vector2f mousePos, int& row, int& col)
{
    // guardamos la posicion del mouse a posicion en matriz
    row = (int)(mousePos.y / this->cellHeight);
    col = (int)(mousePos.x / this->cellWidth);
}

//METODOS PUBLICOS

void TankManager::moveTank(sf::Vector2f mousePos, int currentPlayer)
{
    // convertir pixeles a coordenadas de la matriz
	int mouseRow;
	int mouseCol;
	this->mouseClickToCoords(mousePos, mouseRow, mouseCol);

	// verificar que la celda no sea uun obstaculo
	if (!this->gameMap->isCellFree(mouseRow, mouseCol)) {
		// aqui quiero agregar un aviso de pq no se pudo mover el tanque ahi. Dale okok 
		return;
	}
	// verificar que no haya un tanque en la posicion
	if (this->isThereATank(mouseRow, mouseCol)) {
		return;
	}

	// obtener el tanque a mover 
	Tank* tankToMove = this->players[currentPlayer]->getSelectedTank();

	// verificar que la diferencia entre las posiciones esta en el rango permitido
	int rowDiff = abs(mouseRow - tankToMove->getCurrentRow());
	int colDiff = abs(mouseCol - tankToMove->getCurrentCol());

	if (rowDiff > MAX_MOVE_RADIUS || colDiff > MAX_MOVE_RADIUS) {
		return;
	}

	this->activeTank = tankToMove;

	// convertir indices del mapa a indices en la matriz de adyacencia del grafo
	int currentIndex = this->gameMap->toIndex(tankToMove->getCurrentRow(), tankToMove->getCurrentCol());
	int GoalIndex = this->gameMap->toIndex(mouseRow, mouseCol);

	//bloquemos los otros tanques para detectarlos como "obstaculos"
	this->blockOtherTanks(tankToMove);
	//selecciona el algoritmo a utilizar de acuerdo al color del tanque y se lo da a this->activeTank, luego de bloquearlos para que haga el path bien
	this->selectPathAlgorithm(currentIndex, GoalIndex, currentPlayer);
	//los desbloqueamos luego de ya calcular el movimiento
	this->unblockOtherTanks(tankToMove);
	// reiniciar tanque seleccionado(turnos)
	this->players[currentPlayer]->deselectTank();
}

//Anima el sprite de el tanque
void TankManager::AnimateTankMove()
{
    int* path = this->activeTank->getPathToGo();

    // calcular cual es la celda destino y su posicion en pixeles
    int goalCell = path[this->activeTank->getPathIndex()];
    int goalRow = this->gameMap->toRow(goalCell);
    int goalCol = this->gameMap->toCol(goalCell);

    // convertir la fila y columna a pixeless
    float goalX = goalCol * this->cellWidth;
    float goalY = goalRow * this->cellHeight;

    // calcular la diferencia entre la posicion actual y el destino
    sf::Vector2f currentPos = this->activeTank->getSpritePosition();
    float dx = goalX - currentPos.x;
    float dy = goalY - currentPos.y;

    // calcular la distancia al destino con distancia euclidiana
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < TANK_SPEED) {
        // colocar el sprite en la celda, actualizar posicion logica y aumentar la cantidad de celdas recorridas
        this->activeTank->setPosition(goalX, goalY);
        this->activeTank->setCurrentRow(goalRow);
        this->activeTank->setCurrentCol(goalCol);
        this->activeTank->incrementPathIndex();

        // verificar si ya se recorrio todo el path
        if (this->activeTank->getPathIndex() >= this->activeTank->GetPathSize()) {
            this->activeTank->setIsMoving(false);
            this->activeTank->clearPath();
        }

    }
    else {

        // obtener direccion movimiento
        // dividir entre la distancia convierte dx y dy a valores entre -1 y 1
        float normalX = dx / distance;
        float normalY = dy / distance;

        // mover el sprite TANK_SPEED pixeles en la direccion correcta
        this->activeTank->moveSprite(normalX * TANK_SPEED, normalY * TANK_SPEED);

    } 
}

// Verifica si el jugador hizo click en alguno de sus tanques
void TankManager::TankSelection(sf::Vector2f mousePos, int currentPlayer)
{
    Tank* tank1 = this->players[currentPlayer]->getTank(0);
    Tank* tank2 = this->players[currentPlayer]->getTank(1);

    //Si el click es dentro del area del tanque 1/2 lo selecciona
    if (tank1->getArea().contains(mousePos) && tank1->getIsAlive()) {
        this->players[currentPlayer]->selectTank(tank1);
    }
    if (tank2->getArea().contains(mousePos) && tank2->getIsAlive()) {
        this->players[currentPlayer]->selectTank(tank2);
    }
}

//Verifica si hay un tanque en la casilla
bool TankManager::isThereATank(int row, int col)
{
    //Si alguno de los 4 tanques esta en la posicion es true
    for (int i = 0; i < 4; i++) {
        if (this->tanks[i]->getCurrentRow() == row && this->tanks[i]->getCurrentCol() == col) {
            return true;
        }
    }
    return false;
}

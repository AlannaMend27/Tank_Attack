#include "bulletController.h"

// constructor
BulletController::BulletController(Map* map, Tank** tanks, Player** players,
    sf::RenderWindow* window, sf::Vector2u windowSize,
    float cellWidth, float cellHeight)
{
    // guardar referencias a los datos del juego
    this->gameMap = map;
    this->tanks = tanks;
    this->players = players;
    this->window = window;
    this->windowSize = windowSize;
    this->cellWidth = cellWidth;
    this->cellHeight = cellHeight;
	this->impactOnTank = false;

    // inicializar punteros en nullptr
    this->activeBullet = nullptr;
    this->AlgAStar = nullptr;
    this->AlgLineOfSight = nullptr;
}

// destructor
BulletController::~BulletController()
{
    // liberar memoria de la bala y algoritmos si existen
    delete this->activeBullet;
    delete this->AlgAStar;
    delete this->AlgLineOfSight;
}

// verifica si hay una bala moviendose
bool BulletController::isAnimating()
{
	return this->activeBullet != nullptr && this->activeBullet->getIsMoving();
}

// verifica si la bala termino su movimiento y debe eliminarse
bool BulletController::isReadyToDelete()
{
	return this->activeBullet != nullptr && !this->activeBullet->getIsMoving();
}

// limpia la bala activa
void BulletController::clearBullet()
{
	delete this->activeBullet;
	this->activeBullet = nullptr;
}

// getter de la bala activa
Bullet* BulletController::getActiveBullet()
{
	return this->activeBullet;
}

// dispara desde el tanque seleccionado hasta donde se haga click (derecho)
void BulletController::shootBullet(sf::Vector2f mousePos, int currentPlayer)
{

	// convertir click a coordenadas
	int goalRow = (int)(mousePos.y / this->cellHeight);
	int goalCol = (int)(mousePos.x / this->cellWidth);

	//obtener el tanque que dispara y sus coords
	Tank* shootingTank = this->players[currentPlayer]->getSelectedTank();

	//si no hay tanque seleccionado no disparar
	if (shootingTank == nullptr) {
		return;
	}

	int tankRow = shootingTank->getCurrentRow();
	int tankCol = shootingTank->getCurrentCol();

	// si el jugador tiene precision de ataque activa, y la celda no es un muro usar A* en vez de linea vista
	if (this->players[currentPlayer]->getActivePowerUp() == (int)PowerUpType::attackPrecision && this->gameMap->isCellFree(goalRow, goalCol)) {

		//crea bala y objetivo
		this->activeBullet = new Bullet(tankRow, tankCol, this->windowSize, this->window);
		this->activeBullet->setGoal(goalRow, goalCol);
		//con A*
		this->applyAttackPrecision(tankRow, tankCol, goalRow, goalCol, currentPlayer);
		this->players[currentPlayer]->deselectTank();
		return;
	}

	//Disparo normal con linea vista
	this->AlgLineOfSight = new LineOfSight(this->gameMap->getMapMatrix());
	//calcular el path con linea vista desde el tanque que disparo hasta el goal
	this->AlgLineOfSight->LineOfSightAlgorithm(tankRow, tankCol, goalRow, goalCol);

	// para guardar el path temporalmente
	int* path = this->AlgLineOfSight->getPath();
	int sizeOfPath = this->AlgLineOfSight->getPathSize();

	// si el camino es inalcanzabkle no se dispara
	if (path == nullptr || sizeOfPath == 0) {
		delete this->AlgLineOfSight;
		this->AlgLineOfSight = nullptr;
		return;
	}

	//creamos la bala donde esta el tanque que dispara y el camino que va a seguir
	this->activeBullet = new Bullet(tankRow, tankCol, this->windowSize, this->window);
	this->activeBullet->setPathToGo(path, sizeOfPath);
	// guardamos donde se hace click, para saber si llego al goal o choco con algo
	this->activeBullet->setGoal(goalRow, goalCol);

	// restamos la posicion del tanque para obtener la diferencia (cuanto se movio en cada eje)
	int dirRow = this->AlgLineOfSight->getLastRow() - tankRow;
	int dirCol = this->AlgLineOfSight->getLastCol() - tankCol;

	//si es la misma del tanque no disparar
	if (dirRow == 0 && dirCol == 0) {
		delete this->activeBullet;
		this->activeBullet = nullptr;
		delete this->AlgLineOfSight;
		this->AlgLineOfSight = nullptr;
		return;
	}


	// normalizamos las direcciones en ambos ejes a -1, 0 o 1
	// si dirRow es -1 = va hacia arriba, 0 = no se mueve en filas, 1 = va hacia abajo
	// dirCol es -1 = va hacia la izquierda, 0 = no se mueve en columnas, 1 = va hacia la derecha
	// ejemplo: dirRow = 5 → 5/5 = 1 (iba hacia abajo)
	//          dirRow = -3 → -3/3 = -1 (iba hacia arriba)
	//          dirRow = 0 → se queda en 0 (no se movia en ese eje)
	// la idea es guardar esta direcion para que en calculatenext bounce le demos un giro de 90 grados 

	if (dirRow != 0) {
		dirRow = dirRow / abs(dirRow);
	}

	if (dirCol != 0) {
		dirCol = dirCol / abs(dirCol);
	}

	// aqui la guardamos
	this->activeBullet->setDir(dirRow, dirCol);

	//deseleccionamos el tanque y ponemos modo mover de nuevo
	this->players[currentPlayer]->deselectTank();
}

//calcula el siguiente segmento de la bala luego de pegar con un muro
void BulletController::calculateNextBounce()
{
	//posicion actual de la bala, desde aqui empiesza el nuevo segmento
	int currentRow = this->activeBullet->getCurrentRow();
	int currentCol = this->activeBullet->getCurrentCol();


	// si la posicion actual no es valida o es un obstaculo desaparece
	if (!this->gameMap->isPositionValid(currentRow, currentCol) || !this->gameMap->isCellFree(currentRow, currentCol)) {
		this->activeBullet->setIsMoving(false);
		this->activeBullet->clearPath();
		return;
	}

	// asegura que los tanques no esten bloqueados como obstaculos, sin esto las balas rebotan en los tanwqques
	for (int i = 0; i < 4; i++) {
		this->gameMap->unblockMapNode(this->tanks[i]->getCurrentRow(), this->tanks[i]->getCurrentCol());
	}

	//obtenemos la direccion que traia la bala antes del rebote
	int dirRow = this->activeBullet->getDirRow();
	int dirCol = this->activeBullet->getDirCol();

	//rota 90 grados siempre
	int newDirRow = dirCol;
	int newDirCol = -dirRow;

	// si estamos en borde superior/inferior, invertir direccion en filas, para que rebote en el borde del mapoa
	if (currentRow == 0 || currentRow == MAP_SIZE - 1) {
		newDirRow = -newDirRow;
	}

	// si estamos en borde izquierdo/derecho, invertir direccion en columnas, para lo mismo
	if (currentCol == 0 || currentCol == MAP_SIZE - 1) {
		newDirCol = -newDirCol;
	}

	//calcula el nuevo objetivo lo mas lejos posible, pero lo limita al tamanio del mapa para uqe no se salga
	int newGoalRow = std::max(0, std::min(MAP_SIZE - 1, currentRow + newDirRow * MAP_SIZE));
	int newGoalCol = std::max(0, std::min(MAP_SIZE - 1, currentCol + newDirCol * MAP_SIZE));

	//si el objetivo es donde ya esta, desaparecer
	if (newGoalRow == currentRow && newGoalCol == currentCol) {
		this->activeBullet->setIsMoving(false);
		this->activeBullet->clearPath();
		return;
	}

	//calculamos el nuevo segmento con linea vista desde donde esta
	LineOfSight* newSegment = new LineOfSight(this->gameMap->getMapMatrix());
	newSegment->LineOfSightAlgorithm(currentRow, currentCol, newGoalRow, newGoalCol);

	int pathSize = newSegment->getPathSize();
	int* originalPath = newSegment->getPath();

	//si no hay camino desaparece
	if (originalPath == nullptr || pathSize == 0) {
		this->activeBullet->setIsMoving(false);
		delete newSegment;
		return;
	}

	// copiar el path antes de borrar el segmento, sin esto destruye el path que le ibamos a dar a la bala
	int* pathCopy = new int[pathSize];
	for (int i = 0; i < pathSize; i++) {
		pathCopy[i] = originalPath[i];
	}
	delete newSegment;

	// actualizar la bala con la nueva direccion, path y sumar el rebote
	this->activeBullet->setDir(newDirRow, newDirCol);
	this->activeBullet->setPathToGo(pathCopy, pathSize);
	this->activeBullet->incrementBounce();
}

//metodo para animar el movimiento de la bala, es casi identico al de tanque
void BulletController::animateBulletMove(int currentPlayer)
{
	int* path = this->activeBullet->getPathToGo();

	//calcular destino y pixeles
	int goalCell = path[this->activeBullet->getPathIndex()];
	int goalRow = this->gameMap->toRow(goalCell);
	int goalCol = this->gameMap->toCol(goalCell);

	// convertir la fila y columna a pixeless
	float goalX = goalCol * this->cellWidth;
	float goalY = goalRow * this->cellHeight;

	// calcular la diferencia entre la posicion actual y el destino
	sf::Vector2f currentPos = this->activeBullet->getSpritePosition();

	float dx = goalX - currentPos.x;
	float dy = goalY - currentPos.y;

	// calcular la distancia al destino con distancia euclidiana
	float distance = std::sqrt(dx * dx + dy * dy);
	
	if (distance < BULLET_SPEED) {
		this->activeBullet->setPosition(goalX, goalY);

		//detectar si la bala pego en un tanque a la hora de ir a goal
		if (this->TankCrash(goalRow, goalCol)) {

			//quita la bala
			this->activeBullet->setIsMoving(false);
			this->activeBullet->clearPath();

			int tankIndex = this->getTankIndex(goalRow, goalCol);
			//verifica si el poder de full power esta activado
			bool fullPower = this->players[currentPlayer]->getActivePowerUp() == (int)PowerUpType::attackPower;
			// aplica el danio al tanque
			this->tanks[tankIndex]->receiveAttack(fullPower);
			this->impactOnTank = true;
			this->affectedTank = tankIndex;

			//aplicamos el poder (quita el poder de la cola)
			if (fullPower == true) {
				this->ClearAttackPower(currentPlayer);
			}

			return;
		}

		this->activeBullet->setCurrentRow(goalRow);
		this->activeBullet->setCurrentCol(goalCol);
		this->activeBullet->incrementPathIndex();

		// verificar si ya se recorrio todo el path
		if (this->activeBullet->getPathIndex() >= this->activeBullet->GetPathSize()) {
			// si ya llego al objetivo desaparece
			if (this->activeBullet->getCurrentRow() == this->activeBullet->getGoalRow() && this->activeBullet->getCurrentCol() == this->activeBullet->getGoalCol()) {

				this->activeBullet->setIsMoving(false);
				this->activeBullet->clearPath();

			}
			// si aun quedan rebotes, calcular el siguiente segmento, si no borrarlo
			else if (this->activeBullet->getBounceCount() < MAX_BULLET_BOUNCES) {
				this->calculateNextBounce();
			}
			else {
				this->activeBullet->setIsMoving(false);
				this->activeBullet->clearPath();
			}
		}

	}
	else {

		// obtener direccion movimiento
		float normalX = dx / distance;
		float normalY = dy / distance;

		// mover el sprite BULLET_SPEED pixeles en la direccion correcta
		this->activeBullet->moveSprite(normalX * BULLET_SPEED, normalY * BULLET_SPEED);

	}
}

// le da el path a seguir al tanque por A* (power up de precision de ataque)
void BulletController::SetAStarPath(int currentIndex, int goalIndex)
{
	int* path;
	int pathSize;

	//llamamos a A*, para el power up de la bala
	this->AlgAStar = new AStar(this->gameMap->getAdjMatrix());
	path = this->AlgAStar->AStarAlgorithm(currentIndex, goalIndex);
	pathSize = this->AlgAStar->getPathSize();

	if (path == nullptr || pathSize == 0) {
		return;
	}

	// copiar el path sin el primer elemento (es el startIndex, la posicion actual de la bala), por que delete no puede borrar path+1
	int newSize = pathSize - 1;
	int* pathCopy = new int[newSize];
	for (int i = 0; i < newSize; i++) {
		pathCopy[i] = path[i + 1];
	}

	//le ponemos el path a la bala
	this->activeBullet->setPathToGo(pathCopy, newSize);
}

//Verifica si hay un tanque en la casilla
bool BulletController::TankCrash(int row, int col)
{
	//Si alguno de los 4 tanques esta en la posicion es true
	for (int i = 0; i < 4; i++) {
		if (this->tanks[i]->getCurrentRow() == row && this->tanks[i]->getCurrentCol() == col) {
			return true;
		}
	}
	return false;
}

// si alguno de los 4 tanques esta en la posicion se devuelve el indice de ese tanque en el arreglo de tanques
int BulletController::getTankIndex(int row, int col)
{
	//Si alguno de los 4 tanques esta en la posicion es true
	for (int i = 0; i < 4; i++) {
		if (this->tanks[i]->getCurrentRow() == row && this->tanks[i]->getCurrentCol() == col) {
			this->affectedTank = i;
			return i;
		}
	}
	return -1;
}

// Le indica a game si una bala la bala disparo al tanque para aplicar el danio y los efectos visuales al tanque
bool BulletController::hadImpact()
{
	// devolver la variable a false
	bool impact = this->impactOnTank;
	this->impactOnTank = false;
	return impact;
}

int BulletController::getAffectedTank()
{
	return this->affectedTank;
}

// power ups relacionados con las balas 

// aplica el power up de precision de ataque (usa A* en vez de linea vista)
void BulletController::applyAttackPrecision(int tankRow, int tankCol, int goalRow, int goalCol, int currentPlayer)
{
	int currentIndex = this->gameMap->toIndex(tankRow, tankCol);
	int goalIndex = this->gameMap->toIndex(goalRow, goalCol);
	this->SetAStarPath(currentIndex, goalIndex);
	this->ClearAttackPower(currentPlayer);
}

// elimina el powerup de la cola de prioridad 
void BulletController::ClearAttackPower(int currentPlayer)
{
	this->players[currentPlayer]->clearActivePowerUp();
}


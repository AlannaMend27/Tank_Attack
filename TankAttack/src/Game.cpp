#include<iostream>
#include <ctime>
#include "Game.h"
#include "map.h"


// constructor y destructor

Game::Game() 
{
	srand(time(0));
	this->initVariables();
	this->initWindow();
	this->gameMap = new Map(MAP_SIZE,this->windowGame,this->windowSize);

}

Game::~Game()
{
	delete this->windowGame;
	delete this->gameMap;

	delete this->tanks[0];
	delete this->tanks[1];
	delete this->tanks[2];
	delete this->tanks[3];

	delete this->players[0];
	delete this->players[1];

	delete this->powerUps[0];
	delete this->powerUps[1];

	delete this->AlgDijkstra;
	delete this->AlgLineOfSight;
	delete this->AlgBFS;
	delete this->AlgAStar;

	delete this->activeBullet;
}

// metodos privados


// inicializar variables
void Game::initVariables()
{
	this->windowGame = nullptr;
	this->State = GameState::menu;
	initMenu();

}

// inicializar ventana
void Game::initWindow()
{
	this->videoMode = sf::VideoMode::getDesktopMode();
	this->windowGame = new sf::RenderWindow(this->videoMode, "Tank Attack Game", sf::Style::Fullscreen);
	this->windowSize = this->windowGame->getSize();
}

// metodos publicos 

// maneja los eventos del juego 
void Game::PollEvents() {
	/*
	 	Metodo PollEvents: Este metodo busca manejar los eventos que recibe la ventana

		- pregunta si hay eventos
		- verifica el tipo de evento con un switch
		- si el evento eindica que se cerro la ventana se cierra.
		- si se identifica que se presiono una tecla, se revisa cual fue
			- Escape: se sale del juego
			- Mouse: revisa donde fue, para ingresar al juego o salir con el boton
			- Tecla C: Cambia el jugador (para probar mientras)
	*/

	while (this->windowGame->pollEvent(gameEvent)) {
		switch (this->gameEvent.type) {
		case sf::Event::Closed:
			this->windowGame->close();
			break;
		case sf::Event::KeyPressed:
			if (this->gameEvent.key.code == sf::Keyboard::Escape) {
				this->windowGame->close();
			}
			//Mientras para probar el cambio de turno con c
			if (this->gameEvent.key.code == sf::Keyboard::C && this->State == GameState::playing) {
				this->switchTurn();
			}
			//Cambio de modo moverse/disparo con v
			if (this->gameEvent.key.code == sf::Keyboard::V && this->State == GameState::playing) {
				if (this->players[this->currentPlayer]->getSelectedTank() != nullptr) {
					//Cambia el modo
					this->tankMode = !this->tankMode;
				}
			}
			//shift poderes
			if (this->gameEvent.key.code == sf::Keyboard::LShift && this->State == GameState::playing) {
				if (this->powerUps[this->currentPlayer]->hasPowerUp()) {
					this->powerUps[this->currentPlayer]->usePowerUp();
					this->switchTurn();
				}
			}
			break;

		case sf::Event::MouseButtonPressed:
			if (gameEvent.type == sf::Event::MouseButtonPressed && gameEvent.mouseButton.button == sf::Mouse::Left) {
				// obtener la posicion
				sf::Vector2f mousePos(gameEvent.mouseButton.x, gameEvent.mouseButton.y);

				// ver si la posicion esta cerca de alguno de los dos botones
				if (this->State == GameState::menu) {
					if (this->playButton.getGlobalBounds().contains(mousePos)) {
						this->State = GameState::playing;
						this->GameInit = false;
					}
					if (this->closeButton.getGlobalBounds().contains(mousePos)) {
						this->windowGame->close();
						break;
					}
				}
				
				if (this->State == GameState::playing) {
					// ver si esta cerca del boton volver
					if (this->backButton.getGlobalBounds().contains(mousePos)) {
						this->State = GameState::menu;
					}

					//nota esto no se si dejarlo, es para bloquear que si hay una bala no deje hacer nada
					if (this->activeBullet != nullptr) {
						break;
					}

					// si no hay tanque seleccionado aun, intentar seleccionar, en modo disparo el click izq no hace nada
					if (!this->tankMode){ 
						if (this->players[this->currentPlayer]->getSelectedTank() == nullptr) {
							this->TankSelection(mousePos);
						}
						else {
							// ya hay tanque seleccionado, este click es el destino
							this->moveTank(mousePos);
						}
					
					}
				}
			}
			//click derecho disparo
			if (gameEvent.mouseButton.button == sf::Mouse::Right) {
				sf::Vector2f mousePos(gameEvent.mouseButton.x, gameEvent.mouseButton.y);

				//Verifica si no hay balas, si esta en modo disparo y si el tanque esta seleccionado
				if (this->State == GameState::playing && this->activeBullet == nullptr && this->tankMode 
					&& this->players[this->currentPlayer]->getSelectedTank() != nullptr) {
						this->shootBullet(mousePos);
				}
			}
		}
	}

}

// update y dender del juego general

void Game::update()
{
	// detectar eventos
	this->PollEvents();

	// decidir donde ir de acuerdo a evento detectado
	switch (this->State) {
	case GameState::menu:
		if (!GameInit) {
			this->initGame();
		}
		this->updateMenu();
		break;

	case GameState::playing:
		this->updateGame();
		break;
	}

}

void Game::render() {

	switch (this->State) {
	case GameState::menu:
		this->renderMenu();
		break;

	case GameState::playing:
		this->renderGame();
		break;
	}

	// dibujar juego

	this->windowGame->display();

}

// metodos de la ventana del menu
void Game::initMenu()
{
	// Este metodo inicializa todas las variables necesarias para dibujar el menu en la ventana.

	// fuente
	this->font.loadFromFile("assets/fonts/determination.ttf");

	//background del menu
	this->background.loadFromFile("assets/textures/menu.png");
	this->backgroundMenu.setTexture(this->background);


	// boton de play
	this->playButton.setSize(sf::Vector2f(300, 110));
	this->playButton.setFillColor(sf::Color(9, 110, 150));
	this->playButton.setOutlineColor(sf::Color(82, 206, 255));
	this->playButton.setOutlineThickness(-5.f);
	this->playButton.setPosition(785, 500);

	this->playText.setFont(font);
	this->playText.setString("Jugar");
	this->playText.setCharacterSize(70);
	this->playText.setFillColor(sf::Color::White);
	this->playText.setPosition(835, 510);

	// boton de salir
	this->closeButton.setSize(sf::Vector2f(300, 60));
	this->closeButton.setFillColor(sf::Color(9, 110, 150));
	this->closeButton.setOutlineColor(sf::Color(82, 206, 255));
	this->closeButton.setOutlineThickness(-5.f);
	this->closeButton.setPosition(785, 670);

	//Texto del boton de salir
	this->closeText.setFont(font);
	this->closeText.setString("Salir");
	this->closeText.setCharacterSize(30);
	this->closeText.setFillColor(sf::Color::White);
	this->closeText.setPosition(885, 680);


}

void Game::updateMenu()
{
	// obtener la posicion actual del mouse relativa a la ventana
	sf::Vector2f mousePos = this->windowGame->mapPixelToCoords(sf::Mouse::getPosition(*this->windowGame));

	// cambiar color de los botones si el mouse esta sobre ellos

	// boton de play
	if (this->playButton.getGlobalBounds().contains(mousePos)) {
		this->playButton.setFillColor(sf::Color(4, 77, 107));
		this->playButton.setOutlineColor(sf::Color(82, 206, 255));
		this->playButton.setOutlineThickness(-5.f);
	}
	else {
		this->playButton.setFillColor(sf::Color(9, 110, 150));
		this->playButton.setOutlineColor(sf::Color(82, 206, 255));
		this->playButton.setOutlineThickness(-5.f);
	}

	// boton de salir
	if (this->closeButton.getGlobalBounds().contains(mousePos)) {
		this->closeButton.setFillColor(sf::Color(4, 77, 107));
		this->closeButton.setOutlineColor(sf::Color(82, 206, 255));
		this->closeButton.setOutlineThickness(-5.f);
	}
	else {
		this->closeButton.setFillColor(sf::Color(9, 110, 150));
		this->closeButton.setOutlineColor(sf::Color(82, 206, 255));
		this->closeButton.setOutlineThickness(-5.f);
	}

}

void Game::renderMenu()
{
	// limpiar la ventana antes de dibujar el frame actual
	this->windowGame->clear();

	// escalar el background para que cubra toda la ventana sin importar la resolucion
	this->backgroundMenu.setScale(
		(float)this->videoMode.width / this->background.getSize().x,   
		(float)this->videoMode.height / this->background.getSize().y   
	);

	// dibujar en orden: fondo primero, luego botones, luego textos encima
	this->windowGame->draw(this->backgroundMenu); 
	this->windowGame->draw(this->playButton); 
	this->windowGame->draw(this->playText);  
	this->windowGame->draw(this->closeButton);
	this->windowGame->draw(this->closeText);  

}

// metodos de la ventana del juego principal
void Game::initGame()
{
	//Este if es para separar lo que siempre es fijo y lo que no, mapa siempre cambia, los botones y donde salen los tanques no
	if (!this->GameInit) {
		//Boton de volver
		this->backButton.setSize(sf::Vector2f(200, 60));
		this->backButton.setFillColor(sf::Color(9, 110, 150));
		this->backButton.setOutlineColor(sf::Color(82, 206, 255));
		this->backButton.setOutlineThickness(-5.f);
		this->backButton.setPosition(860, 20);

		//Texto del boton volver
		this->backText.setFont(font);
		this->backText.setString("Volver");
		this->backText.setCharacterSize(30);
		this->backText.setFillColor(sf::Color::White);
		this->backText.setPosition(905, 30);

		//Tanques en las esquinas (MAP_SIZE -1)
		this->tanks[0] = new Tank(0, 0, this->windowSize, this->windowGame, "assets/textures/tank_0.png", "rosado");
		this->tanks[1] = new Tank(MAP_SIZE - 1, 0, this->windowSize, this->windowGame, "assets/textures/tank_1.png", "amarillo");
		this->tanks[2] = new Tank(0, MAP_SIZE - 1, this->windowSize, this->windowGame, "assets/textures/tank_2.png","azul");
		this->tanks[3] = new Tank(MAP_SIZE - 1, MAP_SIZE - 1, this->windowSize, this->windowGame, "assets/textures/tank_3.png", "rojo");

		//Jugadores, el 1 empieza y tiene los tanques 0 y 1 el jugador 2, tiene los tanques 2 y 3
		this->players[0] = new Player(1, this->tanks[0], this->tanks[1], true);
		this->players[1] = new Player(2, this->tanks[2], this->tanks[3], false);
		this->currentPlayer = 0;

		// poderes, el j1 iconos de izq a der, j2 al reves por eso el true false
		this->powerUps[0] = new PowerUp(this->windowGame, this->windowSize, 10);
		this->powerUps[1] = new PowerUp(this->windowGame, this->windowSize, this->windowSize.x - 85);
		this->turnCount = 0;

		// actualizar bandera
		this->GameInit = true;

		// variables que guardaran la posicion del mouse
		this->mouseRow = 0;
		this->mouseCol = 0;

		//ancho y alto de las celdas
		this->cellWidth = (float)this->windowSize.x / MAP_SIZE;
		this->cellHeight = (float)this->windowSize.y / MAP_SIZE;

		// punteros de algoritmos
		this->AlgDijkstra = nullptr;
		this->AlgLineOfSight = nullptr;
		this->AlgBFS = nullptr;
		this->AlgAStar = nullptr;


		//bala actual
		this->activeBullet = nullptr;
	}
	//Esto siempre para que siempre se genere un mapa nuevo
	this->gameMap->createMap();
}

void Game::switchTurn()
{
	//resetear el modo
	this->tankMode = false;
	// desactiva el turno al jugador actual
	this->players[this->currentPlayer]->setTurn(false);

	//Cambia el turno al otro jugador
	if (this->currentPlayer == 0) {
		this->currentPlayer = 1;
	}
	else {
		this->currentPlayer = 0;
	}

	this->players[this->currentPlayer]->setTurn(true);

	this->turnCount++;
	// cada 4 turnos (se cambia en gameconfig) se genera un powerup
	if (this->turnCount % TURNS_PER_POWERUP == 0) {
		this->powerUps[0]->addRandom();
		this->powerUps[1]->addRandom();
	}
}

//Verifica si hay un tanque en la casilla
bool Game::isThereATank(int row, int col) 
{
	//Si alguno de los 4 tanques esta en la posicion es true
	for (int i = 0; i < 4; i++) {
		if (this->tanks[i]->getCurrentRow() == row && this->tanks[i]->getCurrentCol() == col) {
			return true;
		}
	}
	return false;
}

//Verifica si el jugador hizo click en alguno de sus tanques
void Game::TankSelection(sf::Vector2f mousePos) {

	Tank* tank1 = this->players[this->currentPlayer]->getTank(0);
	Tank* tank2 = this->players[this->currentPlayer]->getTank(1);

	//Si el click es dentro del area del tanque 1/2 lo selecciona
	if (tank1->getArea().contains(mousePos)) {
		this->players[this->currentPlayer]->selectTank(tank1);
	}
	if (tank2->getArea().contains(mousePos)) {
		this->players[this->currentPlayer]->selectTank(tank2);
	}
}

void Game::mouseClickToCoords(sf::Vector2f mousePos, int& row, int& col)
{

	// guardamos la posicion del mouse a posicion en matriz
	row = (int)(mousePos.y / this->cellHeight);
	col = (int)(mousePos.x / this->cellWidth);
	
}

void Game::moveTank(sf::Vector2f mousePos)
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
	Tank* tankToMove = this->players[this->currentPlayer]->getSelectedTank();

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
	this->selectPathAlgorithm(currentIndex, GoalIndex);
	//los desbloqueamos luego de ya calcular el movimiento
	this->unblockOtherTanks(tankToMove);
	this->tankMode = false;
	// reiniciar tanque seleccionado(turnos)
	this->players[this->currentPlayer]->deselectTank();
}

void Game::AnimateTankMove()
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
		this-> activeTank->setCurrentRow(goalRow);
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

//metodo para animar el movimiento de la bala, es casi identico al de tanque
void Game::animateBulletMove()
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
		if (this->isThereATank(goalRow, goalCol)) {

			// nota : aqui podes poner que el tanque reciba danio 

			//quita la bala
			this->activeBullet->setIsMoving(false);
			this->activeBullet->clearPath();
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


void Game::selectPathAlgorithm(int currentIndex, int GoalIndex)
{
	std::string colorTank = this->activeTank->getId();
	if (colorTank == "amarillo" || colorTank == "rojo") {
		// Dijkstra 80% de probabilidad, Linea vista 20%
		int randomNum = rand() % 100;
		if (randomNum < 80) {
			// llamar a dijkstra y establecer el camino que debe de seguir el tanque activo
			this->SetDijkstraPath(currentIndex, GoalIndex);
		}
		else {
			// llamar a linea vista y establecer el camino que debe de seguir el tanque activo
			this->SetLineOfSightPath(currentIndex, GoalIndex);
		}
	}
	else {
		// BFS 50% de probabilidad, Linea vista 50%
		int randomNum = rand() % 100;
		if (randomNum < 50) {
			this->SetBFSPath(currentIndex, GoalIndex);
		}
		else {
			// llamar a linea vista y establecer el camino que debe de seguir el tanque activo
			this->SetLineOfSightPath(currentIndex, GoalIndex);
		}
	}
}

// le da el path a seguir al tanque por el algoritmo de Dijkstra
void Game::SetDijkstraPath(int currentIndex, int GoalIndex)
{

	int* path;
	int sizeOfPath;

	// llamar a dijkstra, obtener el camino y el tamanio del camino
	this->AlgDijkstra = new Dijkstra(this->gameMap->getAdjMatrix());
	path = this->AlgDijkstra->DijkstraAlgorithm(currentIndex, GoalIndex);
	sizeOfPath = this->AlgDijkstra->getPathSize();

	// si el camino es inalcanzable
	if (path == nullptr || sizeOfPath == 0) {
		return;
	}

	// establecer la ruta a seguir del tanque
	this->activeTank->setPathToGo(path, sizeOfPath);
}

// le da el path a seguir al tanque por el algoritmo linea vista y llama a movimiento aleatorio en caso de ser necesario
void Game::SetLineOfSightPath(int currentIndex, int GoalIndex)
{
	this->AlgLineOfSight = new LineOfSight(this->gameMap->getMapMatrix());
	// variables para guardar temporalmente el path
	int* path;
	int sizeOfPath;

	// obtener coordenadas actuales ( en cada iteracion por movimiento aleatorio)
	int tankRow = this->activeTank->getCurrentRow();
	int tankCol = this->activeTank->getCurrentCol();
	int goalRow = this->gameMap->toRow(GoalIndex);
	int goalCol = this->gameMap->toCol(GoalIndex);

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

// le da el path a seguir al tanque por el algoritmo BFS
void Game::SetBFSPath(int currentIndex, int goalIndex) 
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

// le da el path a seguir al tanque por A* (power up de precision de ataque)
void Game::SetAStarPath(int currentIndex, int goalIndex)
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

// aplica el power up de precision de ataque (usa A* en vez de linea vista)
void Game::applyAttackPrecision(int tankRow, int tankCol, int goalRow, int goalCol)
{
	int currentIndex = this->gameMap->toIndex(tankRow, tankCol);
	int goalIndex = this->gameMap->toIndex(goalRow, goalCol);
	this->SetAStarPath(currentIndex, goalIndex);
	this->powerUps[this->currentPlayer]->clearActivePowerUp();
}

// aplica el power up de poder de ataque
void Game::applyAttackPower()
{
	// nota: aqui aplicar el 100% danio
	this->powerUps[this->currentPlayer]->clearActivePowerUp();
}

// aplica el power up de doble turno 
void Game::applyDoubleTurn()
{
	// nota :aqui va la logica
	this->powerUps[this->currentPlayer]->clearActivePowerUp();
}

void Game::applyMovePrecision()
{
	// nota: aqui va la logica
	this->powerUps[this->currentPlayer]->clearActivePowerUp();
}

// mueve aleatoriamnete el tanque dentro de un rango definido
void Game::randomMove(int& randomRow, int& randomCol, int goalRow, int goalCol)
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

// dispara desde el tanque seleccionado hasta donde se haga click (derecho)
void Game::shootBullet(sf::Vector2f mousePos) 
{

	//convertir click a coordenadas
	int goalRow;
	int goalCol;
	this->mouseClickToCoords(mousePos, goalRow, goalCol);

	//obtener el tanque que dispara y sus coords
	Tank* shootingTank = this->players[this->currentPlayer]->getSelectedTank();

	//si no hay tanque seleccionado no disparar
	if (shootingTank == nullptr) {
		return;
	}

	int tankRow = shootingTank->getCurrentRow();
	int tankCol = shootingTank->getCurrentCol();

	// si el jugador tiene precision de ataque activa, y la celda no es un muro usar A* en vez de linea vista
	if (this->powerUps[this->currentPlayer]->getActivePowerUp() == (int)PowerUpType::attackPrecision && this->gameMap->isCellFree(goalRow, goalCol)) {

		//crea bala y objetivo
		this->activeBullet = new Bullet(tankRow, tankCol, this->windowSize, this->windowGame);
		this->activeBullet->setGoal(goalRow, goalCol);
		//con A*
		this->applyAttackPrecision(tankRow, tankCol, goalRow, goalCol);
		this->players[this->currentPlayer]->deselectTank();
		this->tankMode = false;
		return;
	}

	//nota: aqui no bloqueamos los tanques, por que la idea es que reciban danio, la deteccion la hace isthereatank

	//Disparo normal con linea vista
	this->AlgLineOfSight = new LineOfSight(this->gameMap->getMapMatrix()); 
	//calcular el path con linea vista desde el tanque que disparo hasta el goal
	this->AlgLineOfSight->LineOfSightAlgorithm(tankRow, tankCol, goalRow, goalCol);

	// para guardar el path temporalmente
	int* path = this->AlgLineOfSight->getPath();
	int sizeOfPath = this->AlgLineOfSight->getPathSize();

	// si el camino es inalcanzabkle no se dispara
	if (path == nullptr || sizeOfPath == 0) {
		return;
	}

	//creamos la bala donde esta el tanque que dispara y el camino que va a seguir
	this->activeBullet = new Bullet(tankRow, tankCol, this->windowSize, this->windowGame);
	this->activeBullet->setPathToGo(path, sizeOfPath);
	// guardamos donde se hace click, para saber si llego al goal o choco con algo
	this->activeBullet->setGoal(goalRow, goalCol);

	// restamos la posicion del tanque para obtener la diferencia (cuanto se movio en cada eje)
	int dirRow = this->AlgLineOfSight->getLastRow() - tankRow;
	int dirCol = this->AlgLineOfSight->getLastCol() - tankCol;

	//si es la misma del tanque no disparar
	if (dirRow == 0 && dirCol == 0) {
		return;
	}


	// normalizamos las direcciones en ambos ejes a -1, 0 o 1
	// si dirRow es -1 = va hacia arriba, 0 = no se mueve en filas, 1 = va hacia abajo
	// dirCol es -1 = va hacia la izquierda, 0 = no se mueve en columnas, 1 = va hacia la derecha
	// ejemplo: dirRow = 5 → 5/5 = 1 (iba hacia abajo)
	//          dirRow = -3 → -3/3 = -1 (iba hacia arriba)
	//          dirRow = 0 → se queda en 0 (no se movia en ese eje)
	// la idea es guardar esta direcion para que en calculatenext bounce le demos un giro de 90 grados 

	if (dirRow != 0){
		dirRow = dirRow / abs(dirRow);
	}

	if (dirCol != 0) {
		dirCol = dirCol / abs(dirCol);
	}

	// aqui la guardamos
	this->activeBullet->setDir(dirRow, dirCol);

	//deseleccionamos el tanque y ponemos modo mover de nuevo
	this->players[this->currentPlayer]->deselectTank();
	this->tankMode = false;
}

//calcula el siguiente segmento de la bala luego de pegar con un muro
void Game::calculateNextBounce()
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
		this->gameMap->unblockMapNode(this->tanks[i]->getCurrentRow(),this->tanks[i]->getCurrentCol());
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

// Bloquea los nodos de los tanques (para detectarlos como obstaculos), recibe el tanque que no se bloquea
void Game::blockOtherTanks(Tank* tankToExclude) 
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

//Restaura los nodos bloqueados por el metodo anterior, esto se usa despues de calcular el pat
void Game::unblockOtherTanks(Tank* tankToExclude) 
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

void Game::updateGame()
{
	//Esto va mas arriba para que no se puedan mover tanques mientras hay bala
	if (this->activeBullet != nullptr && this->activeBullet->getIsMoving()) {
		this->animateBulletMove();
		return;
	}

	if (this->activeBullet != nullptr && !this->activeBullet->getIsMoving()) {
		delete this->activeBullet;
		this->activeBullet = nullptr;
		this->switchTurn();
	}

	if (this->activeTank != nullptr && this->activeTank->getIsMoving()) {
		this->AnimateTankMove();
	}
	if (this->activeTank != nullptr && !this->activeTank->getIsMoving()) {
		this->activeTank = nullptr;
		this->switchTurn();

		// nota: ya está quitas la linea switchturn por si te molesta, sigue lo de cambiar con c cualquier cosa
	}
	
}

//Renderiza la zona disponible/ no disponible
void Game::renderAvailableMove() 
{
	//si esta en modo disparo (tankMode true) no mostrar
	if (this->tankMode) {
		return;
	}

	Tank* selectedTank = this->players[this->currentPlayer]->getSelectedTank();

	//Sin esto crashea, es por si no hay tanque seleccionado
	if (selectedTank == nullptr) {
		return;
	}

	//Posicion del tanque que se selecciona
	int tankRow = selectedTank->getCurrentRow();
	int tankCol = selectedTank->getCurrentCol();

	// esta es la celda que se va a colorear si esta libre/bloqueada
	sf::RectangleShape cellToColor(sf::Vector2f(this->cellWidth, this->cellHeight));
	cellToColor.setOutlineThickness(-1);
	cellToColor.setOutlineColor(sf::Color(255, 255, 255, 150));

	//Recorre el radio alrededor del tanque segun MAX_MOVE_RADIUS
	for (int row = tankRow - MAX_MOVE_RADIUS; row <= tankRow + MAX_MOVE_RADIUS;row++) {
		for (int col = tankCol - MAX_MOVE_RADIUS; col <= tankCol + MAX_MOVE_RADIUS; col++) {

			if (this->gameMap->isPositionValid(row, col)) {
				//Si esta libre, en blanco
				if (this->gameMap->isCellFree(row, col) && !this->isThereATank(row, col)) {
					cellToColor.setFillColor(sf::Color(175, 238, 238, 80));
				} 
				else {
					//Oscuro si no
					cellToColor.setFillColor(sf::Color(0, 0, 0, 50)); 
				}
				cellToColor.setPosition(col * this->cellWidth, row * this->cellHeight);
				this->windowGame->draw(cellToColor);
			}
		}
	}
}

void Game::renderGame()
{
	// crear mapa
	this->windowGame->clear();
	this->gameMap->drawMap();
	this->windowGame->draw(this->backButton);
	this->windowGame->draw(this->backText);

	this->tanks[0]->createTank();
	this->tanks[1]->createTank();
	this->tanks[2]->createTank();
	this->tanks[3]->createTank();

	float cellWidth = (float)this->windowSize.x / MAP_SIZE;
	float cellHeight = (float)this->windowSize.y / MAP_SIZE;

	sf::RectangleShape highlight(sf::Vector2f(cellWidth, cellHeight));
	highlight.setFillColor(sf::Color::Transparent);
	highlight.setOutlineThickness(-3.f);

	//Si el jugador actual es el 0 se resalta rojo, si no amarillo
	if (this->currentPlayer == 0) {
		highlight.setOutlineColor(sf::Color::Yellow);
	}
	else {
		highlight.setOutlineColor(sf::Color(0, 255, 255, 255));
	}

	//Obtenemos el tanque 0 y 1 de los jugadores
	Tank* tank1 = this->players[this->currentPlayer]->getTank(0);
	Tank* tank2 = this->players[this->currentPlayer]->getTank(1);

	//Ponemos el highlight en donde estan los tanques
	highlight.setPosition(tank1->getCurrentCol() * cellWidth, tank1->getCurrentRow() * cellHeight);
	this->windowGame->draw(highlight);

	highlight.setPosition(tank2->getCurrentCol() * cellWidth, tank2->getCurrentRow() * cellHeight);
	this->windowGame->draw(highlight);

	// mostrar los poderes del jugador actual
	this->powerUps[this->currentPlayer]->drawPowerUp();

	//NOTA ESTE IF ES POR MIENTRAS, es para un indicador del modo disparo, luego lo quitamos 
	// si hay tanque seleccionado en modo disparo, cubrirlo de rojo transparente
	if (this->tankMode && this->players[this->currentPlayer]->getSelectedTank() != nullptr) {

		Tank* selected = this->players[this->currentPlayer]->getSelectedTank();
		sf::RectangleShape shootIndicator(sf::Vector2f(cellWidth, cellHeight));

		shootIndicator.setFillColor(sf::Color(255, 0, 0, 100));
		shootIndicator.setPosition(selected->getCurrentCol() * cellWidth, selected->getCurrentRow() * cellHeight);

		this->windowGame->draw(shootIndicator);
	}

	this->renderAvailableMove();

	// dibujar la bala solo si existe
	if (this->activeBullet != nullptr) {
		this->activeBullet->createBullet();
	}
}

// metodos de acceso a variables privadas

const bool Game::getWindowOpen() const
{
	return this->windowGame->isOpen();
}



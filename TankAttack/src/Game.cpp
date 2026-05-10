#include<iostream>
#include <ctime>
#include "Game.h"
#include "map.h"

using namespace std;

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
			break;

		case sf::Event::MouseButtonPressed:
			if (gameEvent.type == sf::Event::MouseButtonPressed && gameEvent.mouseButton.button == sf::Mouse::Left) {
				// obtener la posición
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

					// si no hay tanque seleccionado aún, intentar seleccionar
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
	// obtener la posición actual del mouse relativa a la ventana
	sf::Vector2f mousePos = this->windowGame->mapPixelToCoords(sf::Mouse::getPosition(*this->windowGame));

	// cambiar color de los botones si el mouse está sobre ellos

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

	// escalar el background para que cubra toda la ventana sin importar la resolución
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
		this->tanks[0] = new Tank(0, 0, this->windowSize, this->windowGame, "assets/textures/tank_0.png", 0);
		this->tanks[1] = new Tank(MAP_SIZE - 1, 0, this->windowSize, this->windowGame, "assets/textures/tank_1.png", 1);
		this->tanks[2] = new Tank(0, MAP_SIZE - 1, this->windowSize, this->windowGame, "assets/textures/tank_2.png",2);
		this->tanks[3] = new Tank(MAP_SIZE - 1, MAP_SIZE - 1, this->windowSize, this->windowGame, "assets/textures/tank_3.png", 3);

		//Jugadores, el 1 empieza y tiene los tanques 0 y 1 el jugador 2, tiene los tanques 2 y 3
		this->players[0] = new Player(1, this->tanks[0], this->tanks[1], true);
		this->players[1] = new Player(2, this->tanks[2], this->tanks[3], false);
		this->currentPlayer = 0;

		// actualizar bandera
		this->GameInit = true;

		// variables que guardaran la posicion del mouse
		this->mouseRow = 0;
		this->mouseCol = 0;

		//ancho y alto de las celdas
		this->cellWidth = (float)this->windowSize.x / MAP_SIZE;
		this->cellHeight = (float)this->windowSize.y / MAP_SIZE;
	}
	//Esto siempre para que siempre se genere un mapa nuevo
	this->gameMap->createMap();
}

void Game::switchTurn()
{
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
		// aqui quiero agregar un aviso de pq no se pudo mover el tanque ahi
		return;
	}
	// verificar que no haya un tanque en la posicion
	if (this->isThereATank(mouseRow, mouseCol)) {
		return;
	}

	// tanque seleccionado
	Tank* tankToMove = this->players[this->currentPlayer]->getSelectedTank();
	this->activeTank = tankToMove;

	// convertir indices del mapa a indices en la matriz de adyacencia del grafo
	int currentIndex = this->gameMap->toIndex(tankToMove->getCurrentRow(), tankToMove->getCurrentCol());
	int GoalIndex = this->gameMap->toIndex(mouseRow, mouseCol);

	// llamar a dijkstra, obtener el camino y el tamanio del camino
	this->AlgDijkstra = new Dijkstra(this->gameMap->getAdjMatrix());
	int* path = this->AlgDijkstra->DijkstraAlgorithm(currentIndex, GoalIndex);
	int sizeOfPath = this->AlgDijkstra->getPathSize();

	// si el camino es inalcanzable
	if (path == nullptr) {
		return;
	}

	// establecer la ruta a seguir del tanque
	tankToMove->setPathToGo(path, sizeOfPath);

	// reiniciar tanque seleccionado(turnos)
	this->players[this->currentPlayer]->deselectTank();

}

void Game::AnimateMoveTank()
{
	int* path = this->activeTank->getPathToGo();
	
	// calcular cual es la celda destino y su posicion en pixeles
	int goalCell = path[this->activeTank->getPathIndex()];
	int goalRow = this->gameMap->toRow(goalCell);
	int goalCol = this->gameMap->toCol(goalCell);

	// convertir la fila y columna a pixeless
	float goalX = goalCol * this->cellWidth;
	float goalY = goalRow * this->cellHeight;

	// obtener la posición actual del sprite en píxeles
	sf::Vector2f currentPos = this->activeTank->getSpritePosition();

	// calcular la diferencia entre la posición actual y el destino
	float dx = goalX - currentPos.x;
	float dy = goalY - currentPos.y;

	// calcular la distancia al destino con distancia euclidiana
	float distance = std::sqrt(dx * dx + dy * dy);

	if (distance < TANK_SPEED) {
		// colocar el sprite en la celda
		this->activeTank->setPosition(goalX, goalY);

		// actualizar posicion logica
		this -> activeTank->setCurrentRow(goalRow);
		this->activeTank->setCurrentCol(goalCol);

		// avanzar al siguiente paso del path
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

		// mover el sprite TANK_SPEED píxeles en la dirección correcta
		this->activeTank->moveSprite(normalX * TANK_SPEED, normalY * TANK_SPEED);

	}

}



void Game::updateGame()
{
	if (this->activeTank != nullptr && this->activeTank->getIsMoving()) {
		this->AnimateMoveTank();
	}
	if (this->activeTank != nullptr && !this->activeTank->getIsMoving()) {
		this->activeTank = nullptr;
		// aqui puede ir la logica de cambiar turnos
	}
	
}

//Renderiza la zona disponible/ no disponible
void Game::renderAvailableMove() 
{

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

	// NOTA : ESTO ES POR MIENTRAS: ES PARA RESALTAR LOS TANQUES SEGUN EL TURNO LINEA 394-419
	float cellWidth = (float)this->windowSize.x / MAP_SIZE;
	float cellHeight = (float)this->windowSize.y / MAP_SIZE;

	sf::RectangleShape highlight(sf::Vector2f(cellWidth, cellHeight));
	highlight.setFillColor(sf::Color::Transparent);
	highlight.setOutlineThickness(-3.f);

	//Si el jugador actual es el 0 se resalta rojo, si no amarillo
	if (this->currentPlayer == 0) {
		highlight.setOutlineColor(sf::Color::Red);
	}
	else {
		highlight.setOutlineColor(sf::Color::Yellow);
	}

	//Obtenemos el tanque 0 y 1 de los jugadores
	Tank* tank1 = this->players[this->currentPlayer]->getTank(0);
	Tank* tank2 = this->players[this->currentPlayer]->getTank(1);

	//Ponemos el highlight en donde estan los tanques
	highlight.setPosition(tank1->getCurrentCol() * cellWidth, tank1->getCurrentRow() * cellHeight);
	this->windowGame->draw(highlight);

	highlight.setPosition(tank2->getCurrentCol() * cellWidth, tank2->getCurrentRow() * cellHeight);
	this->windowGame->draw(highlight);

	this->renderAvailableMove();
}

// metodos de acceso a variables privadas

const bool Game::getWindowOpen() const
{
	return this->windowGame->isOpen();
}



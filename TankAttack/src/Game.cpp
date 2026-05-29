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

	// crear UImanager y dibujar el menu en  la ventana
	this->uiManager = new UIManager(this->windowGame, this->videoMode, this->windowSize);
	this->uiManager->initMenu();

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

	delete this->bulletController;
	delete this->tankManager;
	delete this->uiManager;

}

// metodos privados


// inicializar variables
void Game::initVariables()
{
	this->windowGame = nullptr;
	this->State = GameState::menu;

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
				if (this->players[this->currentPlayer]->hasPowerUp()) {
					this->players[this->currentPlayer]->usePowerUp();
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
					if (this->uiManager->getPlayButtonBounds().contains(mousePos)) {
						this->State = GameState::playing;
						this->GameInit = false;
					}
					if (this->uiManager->getCloseButtonBounds().contains(mousePos)) {
						this->windowGame->close();
						break;
					}
				}
				
				if (this->State == GameState::playing) {
					// ver si esta cerca del boton volver
					if (this->uiManager->getBackButtonBounds().contains(mousePos)) {
						this->State = GameState::menu;
					}

					// si hay una bala no deja hacer nada
					if (this->bulletController->getActiveBullet() != nullptr) {
						break;
					}

					//Si hay un tanque moviendose no deja hacer nada
					if (this->tankManager->isAnimating()) {
						break;
					}

					// si no hay tanque seleccionado aun, intentar seleccionar, en modo disparo el click izq no hace nada
					if (!this->tankMode){ 
						if (this->players[this->currentPlayer]->getSelectedTank() == nullptr) {
							this->tankManager->TankSelection(mousePos, this->currentPlayer);
						}
						else {
							// ya hay tanque seleccionado, este click es el destino
							this->tankManager->moveTank(mousePos, this->currentPlayer);
							this->tankMode = false;
						}
					
					}
				}
				if (this->State == GameState::gameOver) {
					// verificar los botones en la pantalla de game over
					if (this->uiManager->getPlayAgainButtonBounds().contains(mousePos)) {
						// reiniciar el juego
						this->GameInit = false;
						this->State = GameState::playing;
						this->initGame();
						this->gameClock.restart();
					}
					if (this->uiManager->getBackMenuButtonBounds().contains(mousePos)) {
						// volver al menu
						this->GameInit = false;
						this->State = GameState::menu;
					}
				}
			}
			//click derecho disparo
			if (gameEvent.mouseButton.button == sf::Mouse::Right) {
				sf::Vector2f mousePos(gameEvent.mouseButton.x, gameEvent.mouseButton.y);

				//Verifica si no hay balas, si esta en modo disparo y si el tanque esta seleccionado
				if (this->State == GameState::playing && this->bulletController->getActiveBullet() == nullptr && this->tankMode
					&& this->players[this->currentPlayer]->getSelectedTank() != nullptr) {

						// disparar
						this->bulletController->shootBullet(mousePos, this->currentPlayer);

						// deseleccionar el tanque
						this->tankMode = false;                          
						this->players[this->currentPlayer]->deselectTank();
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
		this->uiManager->updateMenu();
		break;

	case GameState::playing:
		this->updateGame();
		this->uiManager->updateClock(this->passedTime);
		break;
	case GameState::gameOver:
		this->uiManager->updateGameOver();
		break;
	}
}

void Game::render() {

	switch (this->State) {
	case GameState::menu:
		this->uiManager->renderMenu();
		break;

	case GameState::playing:
		this->uiManager->renderGame(this->currentPlayer, this->tankMode, this->passedTime);
		break;

    case GameState::gameOver:   
		this->uiManager->renderGame(this->currentPlayer, this->tankMode, this->passedTime);
		this->uiManager->renderGameOver();
		break;
	}

	// dibujar juego

	this->windowGame->display();

}

// metodos de la ventana del juego principal
void Game::initGame()
{
	//Este if es para separar lo que siempre es fijo y lo que no, mapa siempre cambia, los botones y donde salen los tanques no
	if (!this->GameInit) {

		//ancho y alto de las celdas
		this->cellWidth = (float)this->windowSize.x / (MAP_SIZE + MARGIN_WIDTH);
		this->cellHeight = ((float)this->windowSize.y) / MAP_SIZE;

		//Tanques en las esquinas (MAP_SIZE -1)
		this->tanks[0] = new Tank(0, 0, this->windowSize, this->windowGame, "assets/textures/tank_0.png", "rosado");
		this->tanks[1] = new Tank(MAP_SIZE - 1, 0, this->windowSize, this->windowGame, "assets/textures/tank_1.png", "amarillo");
		this->tanks[2] = new Tank(0, MAP_SIZE - 1, this->windowSize, this->windowGame, "assets/textures/tank_2.png","azul");
		this->tanks[3] = new Tank(MAP_SIZE - 1, MAP_SIZE - 1, this->windowSize, this->windowGame, "assets/textures/tank_3.png", "rojo");

		//Jugadores, el 1 empieza y tiene los tanques 0 y 1 el jugador 2, tiene los tanques 2 y 3
		this->players[0] = new Player(1, this->tanks[0], this->tanks[1], 1765, 560, this->windowGame, this->windowSize);
		this->players[1] = new Player(2, this->tanks[2], this->tanks[3], 1765, 1070, this->windowGame, this->windowSize);
		this->currentPlayer = 0;

		// inicializar controladores del juego
		this->bulletController = new BulletController(this->gameMap, this->tanks, this->players, this->windowGame, this->windowSize, this->cellWidth, this->cellHeight);
		this->tankManager = new TankManager(this->gameMap, this->tanks, this->players, this->windowSize, this->cellWidth, this->cellHeight);
		
		// pasar los objetos del juego creados a la UI
		this->uiManager->setGameAttributes(this->gameMap, this->tanks, this->players, this->tankManager, this->bulletController, this->cellWidth, this->cellHeight);

		// inicializar la interfaz grafica del juego
		this->uiManager->initGameUI();
		this->uiManager->initMarginUI();

		this->turnCount = 0;
		this->extraTurns = 0;

		// actualizar bandera
		this->GameInit = true;

		// establecer ningun ganador por el momento
		this->winner = 0;

	}

	//siempre generamos un mapa nuevo
	this->gameMap->createMap();
	this->uiManager->setMapMatrix(this->gameMap->getMapMatrix());

	// siempre reiniciamos el reloj
	this->passedTime = 0.f;
	this->gameClock.restart(); 

}

void Game::switchTurn()
{
	//resetear el modo
	this->tankMode = false;

	// si el poder activo es doble turno, aplicarlo, lo que hace es que agregar 2 extraturns
	if (this->players[this->currentPlayer]->getActivePowerUp() == (int)PowerUpType::doubleTurn) {
		this->applyDoubleTurn();
	}

	//Si tiene turnos extra no cambiamos nada, sigue el mismo jugador
	if (this->extraTurns > 0) {
		this->extraTurns--;
		return;
	}

	//Cambia el turno al otro jugador
	if (this->currentPlayer == 0) {
		this->currentPlayer = 1;
	}
	else {
		this->currentPlayer = 0;
	}

	this->turnCount++;

	// cada 4 turnos se genera un powerup
	if (this->turnCount % TURNS_PER_POWERUP == 0) {
		this->players[0]->addRandomPowerUp();
		this->players[1]->addRandomPowerUp();
	}
}

// aplica el power up de doble turno 
void Game::applyDoubleTurn()
{
	this->extraTurns = 2;
	this->players[this->currentPlayer]->clearActivePowerUp();
}


// este metodo verifica si el juego termina pq un jugador se quedo sin tanques
void Game::IsThereAWin()
{
	// verificar la cantidad de tanqes que tiene cada jugador
	int tanksPlayer1 = this->players[0]->tanksAlive();
	int tanksPlayer2 = this->players[1]->tanksAlive();

	// verificar si alguno se ha quedado sin tanques
	if (tanksPlayer1 == 0) {
		// guardar ganador y establecer fin del juego
		this->winner = 2;
		this->uiManager->initGameOver(this->winner);
		this->State = GameState::gameOver;
	}
	if (tanksPlayer2 == 0) {
		// guardar ganador y establecer fin del juego
		this->winner = 1;
		this->uiManager->initGameOver(this->winner);
		this->State = GameState::gameOver;
	}

}

//este metodo revisa quien fue el ganador al terminarse el tiempao
void Game::WinnerTimeUp()
{
	// verificar la cantidad de tanqes que tiene cada jugador
	int tanksPlayer1 = this->players[0]->tanksAlive();
	int tanksPlayer2 = this->players[1]->tanksAlive();

	// verificar cual jugador tiene mas tanques
	if (tanksPlayer1 > tanksPlayer2) {
		this->winner = 1;
		this->uiManager->initGameOver(this->winner);
		this->State = GameState::gameOver;
	}
	else if (tanksPlayer1 < tanksPlayer2) {
		this->winner = 2;
		this->uiManager->initGameOver(this->winner);
		this->State = GameState::gameOver;
	}
	else {
		// empate
		this->uiManager->initGameOver(this->winner);
		this->State = GameState::gameOver;
	}
}

void Game::updateGame()
{
	// actualizar tiempo
	this->passedTime = this->gameClock.getElapsedTime().asSeconds();

	// verificar si se acabo el tiempo
	if (this->passedTime >= GAME_TIME) {
		this->WinnerTimeUp();
		return;
	}

	//Esto va mas arriba para que no se puedan mover tanques mientras hay bala
	if (this->bulletController->isAnimating()) {

		this->bulletController->animateBulletMove(this->currentPlayer);

	}

	// revisar si hubo un impacto
	if (this->bulletController->hadImpact()) {
		this->uiManager->applyVisualAttackToTank(this->bulletController->getAffectedTank());
	}

	if (this->bulletController->isReadyToDelete()) {
		this->bulletController->clearBullet();
		this->switchTurn();

		// verificar si el juego puede continuar
		this->IsThereAWin();
	}

	//Animacion del tanque 
	if (this->tankManager->isAnimating()) {
		this->tankManager->AnimateTankMove();
	}

	if (this->tankManager->isReadyToSwitch()) {
		this->tankManager->clearActiveTank();
		this->switchTurn();
		// verificar si el juego puede continuar
		this->IsThereAWin();

	}
	
}
// metodos de acceso a variables privadas

const bool Game::getWindowOpen() const
{
	return this->windowGame->isOpen();
}



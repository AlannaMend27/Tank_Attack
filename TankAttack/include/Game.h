#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "tank.h"
#include "GameConfig.h"
#include "Player.h"
#include "dijkstra.h"
#include "lineOfSight.h"
#include "BFS.h"
#include "AStar.h"
#include "bulletController.h"
#include "TankManager.h"
#include "UIManager.h"

enum class GameState {
	menu,
	playing,
	exit,
	gameOver
};

class Game {

private:

	// configuraciones basicas de la ventana
	void initVariables();
	void initWindow();

	// ATRIBUTOS
	// window
	sf::RenderWindow* windowGame;
	sf::Event gameEvent;
	sf::VideoMode videoMode;
	sf::Vector2u windowSize;

	// estado del juego
	GameState State;

	// ATRIBUTOS DEL JUEGO
	bool GameInit = false;

	// mapa
	Map* gameMap;

	// tanques
	Tank* tanks[4] = { nullptr, nullptr, nullptr, nullptr };

	// jugadores 
	Player* players[2] = {nullptr, nullptr};

	// controladores del juego
	BulletController* bulletController = nullptr;
	TankManager* tankManager = nullptr;
	UIManager* uiManager = nullptr;

	// numero del jugador ganador(0 -ninguno, 1 - jugador uno, 2 - jugador dos)
	int winner;

	int extraTurns;

	//0 j1,1 j2
	bool currentPlayer = false;

	// cantidad de turnos que llevan
	int turnCount;

	// alto y ancho de una celda
	float cellWidth;
	float cellHeight;

	//Modo disparo/moverse 0 es moverse 1 disparo
	bool tankMode;

	// reloj de la partida
	sf::Clock gameClock;        
	float passedTime;         

public:
	// constructor y destructor
	Game();
	~Game();

	// metodos del sistema en general
	void run();
	void update();
	void render();

	// manejo de eventos del juego
	void PollEvents();

	// manejo de tecla o click hecho por el usuario
	void handleKeyPressed();
	void handleLeftClick(sf::Vector2f mousePos);
	void handleRightClick(sf::Vector2f mousePos);

	// manejo de eventos del juego
	void handleMenuClick(sf::Vector2f mousePos);
	void handlePlayingClick(sf::Vector2f mousePos);
	void handleGameOverClick(sf::Vector2f mousePos);

	// metodos del juego
	void initGame();
	void updateGame();
	void switchTurn();
	void mouseClickToCoords(sf::Vector2f mousePos, int& row, int& col);

	// metodos relacionados al movimiento de los tanques
	
	//Metodos relacionados a los power uops
	void applyDoubleTurn();

	// Metodos relacionados con la vida de los tanques y la condicion de gane
	void applyVisualAttackToTank(int tankIndex);
	void IsThereAWin();
	void WinnerTimeUp();

	// get del juego
	const bool getWindowOpen() const;

};

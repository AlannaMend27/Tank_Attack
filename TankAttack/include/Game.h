#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "tank.h"
#include "GameConfig.h"
#include "Player.h"
#include "dijkstra.h"
#include "lineOfSight.h"
#include "BFS.h"
#include "Bullet.h"
#include "AStar.h"
#include "PowerUp.h"

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

	// ATRIBUTOS DEL MENU
	
	// fondos del juego
	sf::Texture background;
	sf::Sprite backgroundMenu;

	// fondo del margen
	sf::Texture backMargin;
	sf::Sprite backgroundMargin;

	// fuente principal
	sf::Font font;

	//botones y texto
	sf::RectangleShape playButton;
	sf::RectangleShape closeButton;
	sf::Text playText;
	sf::Text closeText;

	// elemntos del margen
	sf::Text TitleText1;
	sf::Text TitleText2;
	sf::Text Player1Text1;
	sf::Text Player1Text2;
	sf::Text Player1Text3;
	sf::Text Player1Text4;

	sf::Text Player2Text1;
	sf::Text Player2Text2;
	sf::Text Player2Text3;
	sf::Text Player2Text4;

	// imagenes de tanques y su vida.
	sf::Texture pinkTankTexture;
	sf::Sprite pinkTank;
	sf::Texture pinkTankLifeTexture;
	sf::Sprite pinkTankLife;

	sf::Texture yellowTankTexture;
	sf::Sprite yellowTank;
	sf::Texture yellowTankLifeTexture;
	sf::Sprite yellowTankLife;

	sf::Texture redTankTexture;
	sf::Sprite redTank;
	sf::Texture redTankLifeTexture;
	sf::Sprite redTankLife;

	sf::Texture blueTankTexture;
	sf::Sprite blueTank;
	sf::Texture blueTankLifeTexture;
	sf::Sprite blueTankLife;

	// elementos del gameOver
	sf::RectangleShape gameOverBackground;
	sf::RectangleShape gameOverBox;
	sf::Text winnerText;
	sf::Text congratsText;
	sf::RectangleShape playAgainButton;
	sf::Text playAgainText;
	sf::RectangleShape backMenuButton;
	sf::Text backMenuText;


	// ATRIBUTOS DEL JUEGO
	sf::RectangleShape backButton;
	sf::Text backText;
	bool GameInit = false;

	// mapa
	Map* gameMap;

	// tanques
	Tank* tanks[4] = { nullptr, nullptr, nullptr, nullptr };
	Tank* activeTank = nullptr;
	
	// bala
	Bullet* activeBullet = nullptr;

	// jugadores 
	Player* players[2] = {nullptr, nullptr};

	// numero del jugador ganador(0 -ninguno, 1 - jugador uno, 2 - jugador dos)
	int winner;

	// poderes
	PowerUp* powerUps[2];

	//0 j1,1 j2
	int currentPlayer = 0;

	// cantidad de turnos que llevan
	int turnCount;

	// posicion de los clicks en la matriz
	int mouseRow;
	int mouseCol;

	// alto y ancho de una celda
	float cellWidth;
	float cellHeight;

	//Modo disparo/moverse 0 es moverse 1 disparo
	bool tankMode;

	// algoritmos de busqueda de camino mas corto
	Dijkstra* AlgDijkstra;
	LineOfSight* AlgLineOfSight;
	BFS* AlgBFS;
	AStar* AlgAStar;

	// reloj de la partida
	sf::Clock gameClock;        
	float passedTime;         
	sf::Text timerText;
	sf::RectangleShape backClock;

public:
	// constructor y destructor
	Game();
	~Game();

	// acceder a variables
	const bool getWindowOpen() const;

	// metodos del juego
	void run();
	void PollEvents();
	void update();
	void render();

	// metodos del menu
	void initMenu();
	void updateMenu();
	void renderMenu();

	// metodos del juego
	void initGame();
	void updateGame();
	void renderGame();
	void renderMargin();
	void renderAvailableMove();
	void switchTurn();
	void TankSelection(sf::Vector2f mousePos);
	void mouseClickToCoords(sf::Vector2f mousePos, int& row, int& col);
	void shootBullet(sf::Vector2f mousePos);
	void blockOtherTanks(Tank* tankToExclude);
	void unblockOtherTanks(Tank* tankToExclude);

	// metodos relacionados al movimiento de los tanques
	void moveTank(sf::Vector2f mousePos);
	void AnimateTankMove();
	void selectPathAlgorithm(int currentIndex, int GoalIndex);
	void SetDijkstraPath(int currentIndex, int GoalIndex);
	void SetLineOfSightPath(int currentIndex, int GoalIndex);
	void SetBFSPath(int currentIndex, int GoalIndex);
	void randomMove(int& randomRow, int& randomCol,int goalRow, int goalCol);
	bool isThereATank(int row, int col);
	int tankInPos(int row, int col);

	//Metodos relacionados al movimiento de las balas
	void animateBulletMove();
	void calculateNextBounce();
	void SetAStarPath(int currentIndex, int goalIndex);
	
	//Metodos relacionados a los power uops
	void applyAttackPrecision(int tankRow, int tankCol, int goalRow, int goalCol);
	void applyAttackPower();
	void applyDoubleTurn();
	void applyMovePrecision();

	// Metodos relacionados con la vida de los tanques y la condicion de gane
	void applyAttackToTank(int tankIndex);
	void IsThereAWin();
	void WinnerTimeUp();
	void updateClock();
	void initGameOver();
	void updateGameOver();
	void renderGameOver();

};

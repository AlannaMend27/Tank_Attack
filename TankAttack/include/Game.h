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

enum class GameState {
	menu,
	playing,
	exit
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

	// fuente principal
	sf::Font font;

	//botones y texto
	sf::RectangleShape playButton;
	sf::RectangleShape closeButton;
	sf::Text playText;
	sf::Text closeText;

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

	//0 j1,1 j2
	int currentPlayer = 0;


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

	//Metodos relacionados al movimiento de las balas
	void animateBulletMove();
	void calculateNextBounce();

};

#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"

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



};

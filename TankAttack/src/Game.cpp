#include<iostream>
#include "Game.h"
#include "map.h"

using namespace std;

// constructor y destrcutor

Game::Game() 
{
	this->initVariables();
	this->initWindow();
	this->gameMap = new Map(15,this->windowGame,this->windowSize);

}

Game::~Game()
{
	delete this->windowGame;
	delete this->gameMap;

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
	 	Metodo PollEvents: Este metodo busca manejar los eventos que recibe la venatana

		- pregunta si hay eventos
		- verifica el tipo de evento con un switch
		- si el evento eindica que se cerro la ventana se cierra.
		- si se identifica que se presiono una tecla, se revisa cual fue
			- Escape: se sale del juego
			- Mouse: revisa donde fue, para ingresar al juego o salir con el boton
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
			break;

		case sf::Event::MouseButtonPressed:
			if (gameEvent.type == sf::Event::MouseButtonPressed && gameEvent.mouseButton.button == sf::Mouse::Left) {
				// obtener la posición
				sf::Vector2f mousePos(gameEvent.mouseButton.x, gameEvent.mouseButton.y);

				// ver si la posicion esta cerca de alguno de los dos botones
				if (this->State == GameState::menu) {
					if (this->playButton.getGlobalBounds().contains(mousePos)) {
						this->State = GameState::playing;
					}
					if (this->closeButton.getGlobalBounds().contains(mousePos)) {
						this->windowGame->close();
						break;
					}
				}
				else {
					this->State = GameState::menu;
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

	// diabujar juego

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
	//Boton de volver
	this->backButton.setSize(sf::Vector2f(200, 60));
	this->backButton.setFillColor(sf::Color(0, 100, 200));
	this->backButton.setPosition(300, 400);

	//Texto del boton volver
	this->backText.setFont(font);
	this->backText.setString("Volver");
	this->backText.setCharacterSize(30);
	this->backText.setFillColor(sf::Color::White);
	this->backText.setPosition(340, 410);

	// actualizar bandera
	this->GameInit = true;


}

void Game::updateGame()
{
	// en esta parte ira la logica del cambio d eposicion de tanques y todo eso
}

void Game::renderGame()
{
	this->gameMap->createMap();
	this->windowGame->draw(this->backButton);
	this->windowGame->draw(this->backText);

	// crear mapa


}



// metodos de acceso a variables privadas

const bool Game::getWindowOpen() const
{
	return this->windowGame->isOpen();
}



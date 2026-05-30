#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "tank.h"
#include "player.h"
#include "GameConfig.h"

// forward declaration para evitar includes circulares
class TankManager;
class BulletController;

class UIManager {
private:

    // referencias del juego
    sf::RenderWindow* window;
    sf::Vector2u windowSize;
    sf::VideoMode videoMode;
    Map* gameMap;
    Tank** tanks;
    Player** players;
    TankManager* tankManager;
    BulletController* bulletController;
    float cellWidth;
    float cellHeight;

    // fuente
    sf::Font font;

    // Atribtos necesarios para el menu
    sf::Texture background;
    sf::Sprite backgroundMenu;
    sf::RectangleShape playButton;
    sf::RectangleShape closeButton;
    sf::Text playText;
    sf::Text closeText;

    // Atributos necesarios para el juego
    sf::RectangleShape backButton;
    sf::Text backText;

    // atributos del mapa
    sf::Texture wall;
    sf::Sprite wallMap;
    int** mapMatrix;

    sf::Texture backgroundMapText;
    sf::Sprite backgroundMap;

    // fondo del margen lateral
    sf::Texture backMargin;
    sf::Sprite backgroundMargin;

    // textos del margen
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

    // sprites de tanques y vidas en el margen
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

    // reloj
    sf::Text timerText;
    sf::RectangleShape backClock;

    // Atributos del game Over
    sf::RectangleShape gameOverBackground;
    sf::RectangleShape gameOverBox;
    sf::Text winnerText;
    sf::Text congratsText;
    sf::RectangleShape playAgainButton;
    sf::Text playAgainText;
    sf::RectangleShape backMenuButton;
    sf::Text backMenuText;

public:
    // constructor y destructor
    UIManager(sf::RenderWindow* window, sf::VideoMode videoMode, sf::Vector2u windowSize);
    ~UIManager();

    // metodos la interfaz del menu
    void initMenu();
    void updateMenu();
    void renderMenu();

    // metodos del juego
    void setGameAttributes(Map* gameMap,Tank** tanks, Player** players,TankManager* tankManager, BulletController* bulletController, float cellWidth, float cellHeight);
    void setMapMatrix(int** mapMatrix);
    void initGameUI();   
    void initMarginUI();
    void renderGame(int currentPlayer, bool tankMode, float passedTime);
    void renderMargin();
    void renderAvailableMove(int currentPlayer, bool tankMode);
    void updateClock(float passedTime);

    // creacion del menu
    void initMap();
    void renderMap();

    // metdos que muestran la pantalla de gane cuando termina el juego
    void initGameOver(int winner);
    void updateGameOver();
    void renderGameOver();

    // actualiza los sprites de vida en el margen
    void applyVisualAttackToTank(int tankIndex);

    // getters de botones que Game necesita para detectar clicks
    sf::FloatRect getPlayButtonBounds();
    sf::FloatRect getCloseButtonBounds();
    sf::FloatRect getBackButtonBounds();
    sf::FloatRect getPlayAgainButtonBounds();
    sf::FloatRect getBackMenuButtonBounds();

    // getter de la fuente para que Game la use en otros textos
    sf::Font& getFont();
};
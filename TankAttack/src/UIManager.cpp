#include "UIManager.h"
#include "TankManager.h"
#include "bulletController.h"

// constructor y destructor 
UIManager::UIManager(sf::RenderWindow* window, sf::VideoMode videoMode, sf::Vector2u windowSize)
{
    this->window = window;
    this->videoMode = videoMode;
    this->windowSize = windowSize;

    // inicializar variables cuyos valores se estableceran despues
    this->gameMap = nullptr;
    this->tanks = nullptr;
    this->players = nullptr;
    this->tankManager = nullptr;
    this->bulletController = nullptr;
    this->cellWidth = 0;
    this->cellHeight = 0;
}

UIManager::~UIManager() {}

// ventana del menu
// Este metodo inicializa todas las variables necesarias para dibujar el menu en la ventana.
void UIManager::initMenu()
{
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

// actualiza el color de los botones en caso de estar o no presionados
void UIManager::updateMenu()
{
    // obtener la posicion actual del mouse relativa a la ventana
    sf::Vector2f mousePos = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));

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

// dibuja los elementos de la ventana de menu
void UIManager::renderMenu()
{
    this->window->clear();

    this->backgroundMenu.setScale(
        (float)this->videoMode.width / this->background.getSize().x,
        (float)this->videoMode.height / this->background.getSize().y
    );

    this->window->draw(this->backgroundMenu);
    this->window->draw(this->playButton);
    this->window->draw(this->playText);
    this->window->draw(this->closeButton);
    this->window->draw(this->closeText);
}

// ventana del juego

// Una vez creados los atributos del juego en game, se reciben en la UI
void UIManager::setGameAttributes(Map* gameMap, Tank** tanks, Player** players, TankManager* tankManager, BulletController* bulletController, float cellWidth, float cellHeight)
{
    this->gameMap = gameMap;
    this->tanks = tanks;
    this->players = players;
    this->tankManager = tankManager;
    this->bulletController = bulletController;
    this->cellWidth = cellWidth;
    this->cellHeight = cellHeight;
}

void UIManager::setMapMatrix(int** mapMatrix)
{
    this->mapMatrix = mapMatrix;
}

// recibe la nueva matriz del mapa cada vez que se inicia una nueva partida

// se inicializan los elementos de la ventana del menu
void UIManager::initGameUI()
{
    // boton volver
    this->backButton.setSize(sf::Vector2f(100, 30));
    this->backButton.setFillColor(sf::Color(9, 110, 150));
    this->backButton.setOutlineColor(sf::Color(82, 206, 255));
    this->backButton.setOutlineThickness(-1.f);
    this->backButton.setPosition(1760, 1165);

    this->backText.setFont(font);
    this->backText.setString("Volver");
    this->backText.setCharacterSize(16);
    this->backText.setFillColor(sf::Color::White);
    this->backText.setPosition(1785, 1170);

    // reloj
    this->backClock.setSize(sf::Vector2f(100, 50));
    this->backClock.setFillColor(sf::Color(9, 110, 150));
    this->backClock.setOutlineColor(sf::Color(82, 206, 255));
    this->backClock.setOutlineThickness(-1.f);
    this->backClock.setPosition(800, 10);

    this->timerText.setFont(this->font);
    this->timerText.setCharacterSize(30);
    this->timerText.setFillColor(sf::Color::White);
    this->timerText.setPosition(820, 18);

    // iniicalizar tambien el mapa que es parte del juego
    this->initMap();

}

void UIManager::initMarginUI()
{
    // fondo del margen
    this->backMargin.loadFromFile("assets/textures/marginBack.png");
    this->backgroundMargin.setTexture(this->backMargin);

    // texto (Tank Attack)
    this->TitleText1.setFont(font);
    this->TitleText1.setString("TANK");
    this->TitleText1.setCharacterSize(50);
    this->TitleText1.setFillColor(sf::Color(0, 211, 217));
    this->TitleText1.setPosition(1750, 10);

    this->TitleText2.setFont(font);
    this->TitleText2.setString("ATTACK!");
    this->TitleText2.setCharacterSize(50);
    this->TitleText2.setFillColor(sf::Color(0, 211, 217));
    this->TitleText2.setPosition(1720, 60);

    // texto (jugadores)

    // jugador 1
    this->Player1Text1.setFont(font);
    this->Player1Text1.setString("Jugador uno");
    this->Player1Text1.setCharacterSize(25);
    this->Player1Text1.setFillColor(sf::Color(0, 211, 217));
    this->Player1Text1.setPosition(1740, 140);

    this->Player1Text2.setFont(font);
    this->Player1Text2.setString("Tanque Rosado");
    this->Player1Text2.setCharacterSize(18);
    this->Player1Text2.setFillColor(sf::Color::White);
    this->Player1Text2.setPosition(1745, 190);

    this->Player1Text3.setFont(font);
    this->Player1Text3.setString("Tanque Amarillo");
    this->Player1Text3.setCharacterSize(18);
    this->Player1Text3.setFillColor(sf::Color::White);
    this->Player1Text3.setPosition(1730, 365);

    this->Player1Text4.setFont(font);
    this->Player1Text4.setString("Power-ups");
    this->Player1Text4.setCharacterSize(18);
    this->Player1Text4.setFillColor(sf::Color::White);
    this->Player1Text4.setPosition(1765, 520);

    // jugador 2
    this->Player2Text1.setFont(font);
    this->Player2Text1.setString("Jugador dos");
    this->Player2Text1.setCharacterSize(25);
    this->Player2Text1.setFillColor(sf::Color(0, 211, 217));
    this->Player2Text1.setPosition(1740, 670);

    this->Player2Text2.setFont(font);
    this->Player2Text2.setString("Tanque Azul");
    this->Player2Text2.setCharacterSize(18);
    this->Player2Text2.setFillColor(sf::Color::White);
    this->Player2Text2.setPosition(1750, 720);

    this->Player2Text3.setFont(font);
    this->Player2Text3.setString("Tanque Rojo");
    this->Player2Text3.setCharacterSize(18);
    this->Player2Text3.setFillColor(sf::Color::White);
    this->Player2Text3.setPosition(1750, 895);

    this->Player2Text4.setFont(font);
    this->Player2Text4.setString("Power-ups");
    this->Player2Text4.setCharacterSize(18);
    this->Player2Text4.setFillColor(sf::Color::White);
    this->Player2Text4.setPosition(1765, 1050);

    // tanques y vida para margen

    // jugador 1
    // tanque rosado (fila 1)
    this->pinkTankTexture.loadFromFile("assets/textures/tank_0.png");
    this->pinkTank.setTexture(this->pinkTankTexture);
    this->pinkTank.setPosition(1750, 220);
    this->pinkTank.setScale(this->cellWidth / this->pinkTankTexture.getSize().x, this->cellHeight / this->pinkTankTexture.getSize().y);

    this->pinkTankLifeTexture.loadFromFile("assets/textures/vida_100.png");
    this->pinkTankLife.setTexture(this->pinkTankLifeTexture);
    this->pinkTankLife.setPosition(1700, 245);
    this->pinkTankLife.setScale(0.150f, 0.150f);

    // tanque amarillo (fila 2)
    this->yellowTankTexture.loadFromFile("assets/textures/tank_1.png");
    this->yellowTank.setTexture(this->yellowTankTexture);
    this->yellowTank.setPosition(1750, 385);
    this->yellowTank.setScale(this->cellWidth / this->yellowTankTexture.getSize().x, this->cellHeight / this->yellowTankTexture.getSize().y);

    this->yellowTankLifeTexture.loadFromFile("assets/textures/vida_100.png");
    this->yellowTankLife.setTexture(this->yellowTankLifeTexture);
    this->yellowTankLife.setPosition(1700, 410);
    this->yellowTankLife.setScale(0.150f, 0.150f);

    // jugador 2
    // tanque azul (fila 2 del jugador 2)
    this->blueTankTexture.loadFromFile("assets/textures/tank_2.png");
    this->blueTank.setTexture(this->blueTankTexture);
    this->blueTank.setPosition(1750, 750);
    this->blueTank.setScale(this->cellWidth / this->blueTankTexture.getSize().x, this->cellHeight / this->blueTankTexture.getSize().y);

    this->blueTankLifeTexture.loadFromFile("assets/textures/vida_100.png");
    this->blueTankLife.setTexture(this->blueTankLifeTexture);
    this->blueTankLife.setPosition(1700, 775);
    this->blueTankLife.setScale(0.150f, 0.150f);

    // tanque rojo (fila 1 del jugador 2)
    this->redTankTexture.loadFromFile("assets/textures/tank_3.png");
    this->redTank.setTexture(this->redTankTexture);
    this->redTank.setPosition(1750, 915);
    this->redTank.setScale(this->cellWidth / this->redTankTexture.getSize().x, this->cellHeight / this->redTankTexture.getSize().y);

    this->redTankLifeTexture.loadFromFile("assets/textures/vida_100.png");
    this->redTankLife.setTexture(this->redTankLifeTexture);
    this->redTankLife.setPosition(1700, 940);
    this->redTankLife.setScale(0.150f, 0.150f);
}

void UIManager::updateClock(float passedTime)
{
    int minutes = (int)passedTime / 60;
    int seconds = (int)passedTime % 60;
    std::string timeStr = std::to_string(minutes) + ":" +
        (seconds < 10 ? "0" : "") + std::to_string(seconds);
    this->timerText.setString(timeStr);
}

void UIManager::renderGame(int currentPlayer, bool tankMode, float passedTime)
{
    // limpiar la ventana
    this->window->clear();

    // renderizar game
    this->renderMap();

    this->tanks[0]->createTank();
    this->tanks[1]->createTank();
    this->tanks[2]->createTank();
    this->tanks[3]->createTank();

    sf::RectangleShape highlight(sf::Vector2f(this->cellWidth, this->cellHeight));
    highlight.setFillColor(sf::Color::Transparent);
    highlight.setOutlineThickness(-3.f);

    //Si el jugador actual es el 0 se resalta rojo, si no amarillo
    if (currentPlayer == 0) {
        highlight.setOutlineColor(sf::Color::Yellow);
    }
    else {
        highlight.setOutlineColor(sf::Color(0, 255, 255, 255));
    }

    //Obtenemos el tanque 0 y 1 de los jugadores
    Tank* tank1 = this->players[currentPlayer]->getTank(0);
    Tank* tank2 = this->players[currentPlayer]->getTank(1);

    //Ponemos el highlight en donde estan los tanques si cada tanque sigue vivo
    if (tank1->getIsAlive()) {
        highlight.setPosition(tank1->getCurrentCol() * this->cellWidth, tank1->getCurrentRow() * this->cellHeight);
        this->window->draw(highlight);
    }

    if (tank2->getIsAlive()) {
        highlight.setPosition(tank2->getCurrentCol() * this->cellWidth, tank2->getCurrentRow() * this->cellHeight);
        this->window->draw(highlight);
    }

    // colocar menu lateral derecho
    this->renderMargin();

    // colocar boton de volver
    window->draw(this->backButton);
    window->draw(this->backText);

    // mostrar los power up de cada jugador
    this->players[0]->drawPowerUp();
    this->players[1]->drawPowerUp();

    if (tankMode && this->players[currentPlayer]->getSelectedTank() != nullptr) {

        Tank* selected = this->players[currentPlayer]->getSelectedTank();
        sf::RectangleShape shootIndicator(sf::Vector2f(this->cellWidth, this->cellHeight));

        shootIndicator.setFillColor(sf::Color(255, 0, 0, 100));
        shootIndicator.setPosition(selected->getCurrentCol() * this->cellWidth, selected->getCurrentRow() * this->cellHeight);

        this->window->draw(shootIndicator);
    }

    this->renderAvailableMove(currentPlayer, tankMode);

    // dibujar bala solo si existe
    if (this->bulletController->getActiveBullet() != nullptr) {
        this->bulletController->getActiveBullet()->createBullet();
    }

    // actualizar reloj en pantalla
    this->window->draw(this->backClock);
    this->window->draw(this->timerText);
}

// dibuja el margen derecho del juego
void UIManager::renderMargin()
{
    //Fondo
    float marginPixels = MARGIN_WIDTH * this->cellWidth;

    // escalar la imagen para que cubra exactamente el margen
    this->backgroundMargin.setScale(
        marginPixels / this->backMargin.getSize().x,
        (float)this->windowSize.y / this->backMargin.getSize().y
    );
    this->backgroundMargin.setPosition(
        this->windowSize.x - marginPixels,
        0.f
    );
    this->window->draw(this->backgroundMargin);

    // texto
    // titulo
    this->window->draw(this->TitleText1);
    this->window->draw(this->TitleText2);

    //info jugadores
    this->window->draw(this->Player1Text1);
    this->window->draw(this->Player1Text2);
    this->window->draw(this->Player1Text3);
    this->window->draw(this->Player1Text4);

    this->window->draw(this->Player2Text1);
    this->window->draw(this->Player2Text2);
    this->window->draw(this->Player2Text3);
    this->window->draw(this->Player2Text4);

    // Tanques y vidas
    this->window->draw(this->yellowTank);
    this->window->draw(this->yellowTankLife);
    this->window->draw(this->pinkTank);
    this->window->draw(this->pinkTankLife);
    this->window->draw(this->redTank);
    this->window->draw(this->redTankLife);
    this->window->draw(this->blueTank);
    this->window->draw(this->blueTankLife);
}

// permite visualizar los movimientos disponibles en el mapa cuando un jugador selecciona un tanque
void UIManager::renderAvailableMove(int currentPlayer, bool tankMode)
{
    //si esta en modo disparo (tankMode true) no mostrar
    if (tankMode) {
        return;
    }

    Tank* selectedTank = this->players[currentPlayer]->getSelectedTank();

    //Verifica que  haya un tanque seleccionado
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
                if (this->gameMap->isCellFree(row, col) && !this->tankManager->isThereATank(row, col)) {
                    cellToColor.setFillColor(sf::Color(175, 238, 238, 80));
                }
                else {
                    //Oscuro si no
                    cellToColor.setFillColor(sf::Color(0, 0, 0, 50));
                }
                cellToColor.setPosition(col * this->cellWidth, row * this->cellHeight);
                this->window->draw(cellToColor);
            }
        }
    }
}
   
// actualiza la vida de los tanques en el margen
void UIManager::applyVisualAttackToTank(int tankIndex)
{
    // cambiar sprite del tanque
    this->tanks[tankIndex]->changeSprite();

    // actualizar vida en el margen de la pantalla
    int life = this->tanks[tankIndex]->getLifeTank();
    std::string tankColor = this->tanks[tankIndex]->getId();
    std::string lifeTextureFile;

    switch (life) {
    case 0:
        lifeTextureFile = "assets/textures/vida_0.png";
        break;
    case 25:
        lifeTextureFile = "assets/textures/vida_25.png";
        break;
    case 50:
        lifeTextureFile = "assets/textures/vida_50.png";
        break;
    case 75:
        lifeTextureFile = "assets/textures/vida_75.png";
        break;
    case 100:
        lifeTextureFile = "assets/textures/vida_100.png";
        break;
    }

    // Asignar la textura según el color del tanque (para no actualizar la vida en pantalla del tanqe quivocado)
    if (tankColor == "amarillo") {
        this->yellowTankLifeTexture.loadFromFile(lifeTextureFile);
        this->yellowTankLife.setTexture(this->yellowTankLifeTexture);
    }
    else if (tankColor == "rosado") {
        this->pinkTankLifeTexture.loadFromFile(lifeTextureFile);
        this->pinkTankLife.setTexture(this->pinkTankLifeTexture);
    }
    else if (tankColor == "rojo") {
        this->redTankLifeTexture.loadFromFile(lifeTextureFile);
        this->redTankLife.setTexture(this->redTankLifeTexture);
    }
    else {
        this->blueTankLifeTexture.loadFromFile(lifeTextureFile);
        this->blueTankLife.setTexture(this->blueTankLifeTexture);
    }

}
// mapa del juego

// inicializa los atributos del mapa
void UIManager::initMap()
{
    // muros
    this->wall.loadFromFile("assets/textures/wall.png");
    this->wallMap.setTexture(this->wall);

    // mapa
    this->backgroundMapText.loadFromFile("assets/textures/mapBackground.png");
    this->backgroundMap.setTexture(this->backgroundMapText);


}

// dibuja el mapa en la ventana una vez haya sido creado logicamente
void UIManager::renderMap()
{

    // calcular el ancho del mapa tomando en cuenta el margen lateral
    float mapWidthPixels = this->windowSize.x - (MARGIN_WIDTH * this->cellWidth);

    // dibujar mapa de fondo
    this->backgroundMap.setScale(
        mapWidthPixels / this->backgroundMapText.getSize().x,
        (float)this->windowSize.y / this->backgroundMapText.getSize().y
    );
    this->window->draw(this->backgroundMap);

    this->wallMap.setScale(cellWidth / this->wall.getSize().x, cellHeight / this->wall.getSize().y);

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (this->mapMatrix[i][j] == 1) {
                this->wallMap.setPosition(
                    j * cellWidth,
                    i * cellHeight
                );
                this->window->draw(this->wallMap);
            }

        }
    }

}



// ventana de game over

//inicializa los atributos de la pantalla que se muestar al terminar el juego (game over)
void UIManager::initGameOver(int winner)
{
    // fondo semitransparente que cubre toda la pantalla
    this->gameOverBackground.setSize(sf::Vector2f(this->windowSize.x, this->windowSize.y));
    this->gameOverBackground.setFillColor(sf::Color(0, 0, 0, 180));
    this->gameOverBackground.setPosition(0.f, 0.f);

    // cuadro central donde va todo el contenido
    float boxWidth = 600.f;
    float boxHeight = 400.f;
    float boxX = (this->windowSize.x - boxWidth) / 2.f;
    float boxY = (this->windowSize.y - boxHeight) / 2.f;

    this->gameOverBox.setSize(sf::Vector2f(boxWidth, boxHeight));
    this->gameOverBox.setFillColor(sf::Color(9, 110, 150));
    this->gameOverBox.setOutlineColor(sf::Color(82, 206, 255));
    this->gameOverBox.setOutlineThickness(-5.f);
    this->gameOverBox.setPosition(boxX, boxY);

    // texto del ganador, se actualiza en checkWinner con el nombre del jugador
    this->winnerText.setFont(this->font);
    this->winnerText.setCharacterSize(50);
    this->winnerText.setFillColor(sf::Color::White);

    if (winner != 0) {
        this->winnerText.setString("Jugador " + std::to_string(winner) + " gana!");
    }
    else {
        this->winnerText.setString("Empate, ambos ganan!");
    }
    // centrar el texto dentro del cuadro
    float textX = boxX + (boxWidth - this->winnerText.getGlobalBounds().width) / 2.f;
    this->winnerText.setPosition(textX, boxY + 40.f);

    // texto de felicitacion debajo del ganador
    this->congratsText.setFont(this->font);
    this->congratsText.setCharacterSize(28);
    this->congratsText.setFillColor(sf::Color(82, 206, 255));
    this->congratsText.setString("Felicidades, excelente partida!");
    float congratsX = boxX + (boxWidth - this->congratsText.getGlobalBounds().width) / 2.f;
    this->congratsText.setPosition(congratsX, boxY + 130.f);

    // boton jugar de nuevo
    this->playAgainButton.setSize(sf::Vector2f(220.f, 60.f));
    this->playAgainButton.setFillColor(sf::Color(9, 110, 150));
    this->playAgainButton.setOutlineColor(sf::Color(82, 206, 255));
    this->playAgainButton.setOutlineThickness(-5.f);
    this->playAgainButton.setPosition(boxX + 50.f, boxY + 300.f);

    this->playAgainText.setFont(this->font);
    this->playAgainText.setCharacterSize(22);
    this->playAgainText.setFillColor(sf::Color::White);
    this->playAgainText.setString("Jugar de nuevo");
    this->playAgainText.setPosition(boxX + 70.f, boxY + 312.f);

    // boton volver al menu
    this->backMenuButton.setSize(sf::Vector2f(220.f, 60.f));
    this->backMenuButton.setFillColor(sf::Color(9, 110, 150));
    this->backMenuButton.setOutlineColor(sf::Color(82, 206, 255));
    this->backMenuButton.setOutlineThickness(-5.f);
    this->backMenuButton.setPosition(boxX + 330.f, boxY + 300.f);

    this->backMenuText.setFont(this->font);
    this->backMenuText.setCharacterSize(22);
    this->backMenuText.setFillColor(sf::Color::White);
    this->backMenuText.setString("Volver al menu");
    this->backMenuText.setPosition(boxX + 355.f, boxY + 312.f);
}

// verifica si el mouse esta sobre un boton en la ventana que se muestra al terminar el juego
void UIManager::updateGameOver()
{
    sf::Vector2f mousePos = this->window->mapPixelToCoords(
        sf::Mouse::getPosition(*this->window)
    );

    // hover boton jugar de nuevo
    if (this->playAgainButton.getGlobalBounds().contains(mousePos)) {
        this->playAgainButton.setFillColor(sf::Color(4, 77, 107));
    }
    else {
        this->playAgainButton.setFillColor(sf::Color(9, 110, 150));
    }

    // hover boton volver al menu
    if (this->backMenuButton.getGlobalBounds().contains(mousePos)) {
        this->backMenuButton.setFillColor(sf::Color(4, 77, 107));
    }
    else {
        this->backMenuButton.setFillColor(sf::Color(9, 110, 150));
    }
}

// renderiza la ventana que se muestra al terminar el juego
void UIManager::renderGameOver()
{
    // encima dibujar un cuadradito semistransparente
    this->window->draw(this->gameOverBackground);

    // dibujar el cuadro y su contenido
    this->window->draw(this->gameOverBox);
    this->window->draw(this->winnerText);
    this->window->draw(this->congratsText);
    this->window->draw(this->playAgainButton);
    this->window->draw(this->playAgainText);
    this->window->draw(this->backMenuButton);
    this->window->draw(this->backMenuText);
}

// getters sobre la ubicacion de los botones del juegos (usad en el metodo PollEvents en game)

sf::FloatRect UIManager::getPlayButtonBounds() {
    return this->playButton.getGlobalBounds();
}
sf::FloatRect UIManager::getCloseButtonBounds() {
    return this->closeButton.getGlobalBounds();
}
sf::FloatRect UIManager::getBackButtonBounds() {
    return this->backButton.getGlobalBounds();
}
sf::FloatRect UIManager::getPlayAgainButtonBounds() {
    return this->playAgainButton.getGlobalBounds();
}
sf::FloatRect UIManager::getBackMenuButtonBounds() {
    return this->backMenuButton.getGlobalBounds();
}
sf::Font& UIManager::getFont() {
    return this->font;
}
#include "PowerUp.h"

// constructor y destructor
PowerUp::PowerUp(sf::RenderWindow* window, sf::Vector2u windowSize, float iconPosX) {
	
	this->window = window;
	this->windowSize = windowSize;
	this->iconPosX = iconPosX;
	this->activePowerUp = -1;

	this->powerUpQueue = new Queue();

	this->doubleTurnTexture.loadFromFile("assets/textures/doubleturn_powerup.png");
	this->movementPrecisionTexture.loadFromFile("assets/textures/movementprecision_powerup.png");
	this->attackPrecisionTexture.loadFromFile("assets/textures/attackprecision_powerup.png");
	this->attackPowerTexture.loadFromFile("assets/textures/attackpower_powerup.png");

	//para poner aparte del actual cuantos mas tiene
	this->font.loadFromFile("assets/fonts/determination.ttf");
	
}

PowerUp::~PowerUp()
{
	delete this->powerUpQueue;
}

//METODOS PRIVADOS

//dibuja los iconos de los power ups en la pantalla, muestra solo el primero, para los demas lleva un contador tipo +cantidad de powerups
void PowerUp::renderIcons()
{
	float iconSize = 75.f;
	//espacio entre el borde inferior y icono
	float margin = 10;

	// si no hay poderes no dibujar nada
	if (this->powerUpQueue->isEmpty()) {
		return;
	}

	// mostrar solo el primer poder de la cola
	sf::Sprite icon;
	int firstPower = this->powerUpQueue->peek();

	//asignar textura segun poder
	if (firstPower == (int)PowerUpType::doubleTurn) {
		icon.setTexture(this->doubleTurnTexture);
	}
	else if (firstPower == (int)PowerUpType::movementPrecision) {
		icon.setTexture(this->movementPrecisionTexture);
	}
	else if (firstPower == (int)PowerUpType::attackPrecision) {
		icon.setTexture(this->attackPrecisionTexture);
	}
	else if (firstPower == (int)PowerUpType::attackPower) {
		icon.setTexture(this->attackPowerTexture);
	}

	//escalamos
	icon.setScale(iconSize / icon.getTexture()->getSize().x, iconSize / icon.getTexture()->getSize().y);

	//posicionamos
	float x = this->iconPosX;
	float y = this->windowSize.y - iconSize - margin;
	icon.setPosition(x, y);
	this->window->draw(icon);

	// si hay mas de un power, mostrar cuantos hay adicionales
	if (this->powerUpQueue->getSize() > 1) {
		sf::Text countText;
		countText.setFont(this->font);
		countText.setString("+" + std::to_string(this->powerUpQueue->getSize() - 1));
		countText.setCharacterSize(40);
		countText.setFillColor(sf::Color::Red);
		countText.setPosition(x-25 + iconSize, y + iconSize / 2);
		this->window->draw(countText);
	}
}

//METODOS PRIVADOS

//agrega un power up aleatorio a la cola
void PowerUp::addRandom() 
{
	int randomType = rand() % 4;
	this->powerUpQueue->enqueue(randomType);
}

// consume el primer poder de la cola, y lo activa para otro turno
void PowerUp::usePowerUp()
{
	if (this->powerUpQueue->isEmpty()) {
		return;
	}

	// sacar el primero y guardarlo como activo
	this->activePowerUp = this->powerUpQueue->dequeue();
}

// dibuja los iconos
void PowerUp::drawPowerUp()
{
	this->renderIcons();
}

// retorna el poder activo
int PowerUp::getActivePowerUp()
{
	return this->activePowerUp;
}

// retorna si hay poderes disponibles en la cola
bool PowerUp::hasPowerUp() 
{
	if (this->powerUpQueue->isEmpty()) {
		return false;
	}
	else {
		return true;
	}
}

// limpia el poder al terminar el turno

void PowerUp::clearActivePowerUp() 
{
	this->activePowerUp = -1;
}
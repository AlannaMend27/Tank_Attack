#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "tank.h"
#include "bullet.h"
#include "player.h"
#include "AStar.h"
#include "LineOfSight.h"
#include "GameConfig.h"

class BulletController {
private:
    // referencias a datos del juego que necesita para funcionar
    Map* gameMap;
    Tank** tanks;           // arreglo de 4 tanques
    Player** players;       // arreglo de 2 jugadores
    sf::RenderWindow* window;
    sf::Vector2u windowSize;

    // bala activa
    Bullet* activeBullet;

    // algoritmos
    AStar* AlgAStar;
    LineOfSight* AlgLineOfSight;

    // celda y tamanio
    float cellWidth;
    float cellHeight;

    // bandera que indica si hubo un impacto en un tanque
    bool impactOnTank;
    int affectedTank;

    // metodos privados
    void calculateNextBounce();
    void SetAStarPath(int currentIndex, int goalIndex);
    bool TankCrash(int row, int col);
    int getTankIndex(int row, int col);

public:
    // constructor y destructor
    BulletController(Map* map, Tank** tanks, Player** players, sf::RenderWindow* window, sf::Vector2u windowSize, float cellWidth, float cellHeight);
    ~BulletController();

    // metodos publicos
    void shootBullet(sf::Vector2f mousePos, int currentPlayer);
    void animateBulletMove(int currentPlayer);
    bool isAnimating();         
    bool isReadyToDelete();     

    // power ups
    void applyAttackPrecision(int tankRow, int tankCol, int goalRow, int goalCol, int currentPlayer);
    void ClearAttackPower(int currentPlayer);

    // limpiar bala al terminar
    void clearBullet();

    // sets y gets
    Bullet* getActiveBullet();
    bool hadImpact();
    int getAffectedTank();
};

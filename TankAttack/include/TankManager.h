#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "tank.h"
#include "player.h"
#include "Dijkstra.h"
#include "BFS.h"
#include "LineOfSight.h"
#include "GameConfig.h"
#include "bulletController.h"

class TankManager {
private:
    // referencias a datos del juego
    Map* gameMap;
    Tank** tanks;          // arreglo de 4 tanques
    Player** players;      // arreglo de 2 jugadores
    sf::Vector2u windowSize;

    // tamanio de celdas
    float cellWidth;
    float cellHeight;

    // tanque activo en animacion
    Tank* activeTank;

    // algoritmos
    Dijkstra* AlgDijkstra;
    BFS* AlgBFS;
    LineOfSight* AlgLineOfSight;

    // metodos privados de pathfinding
    void SetDijkstraPath(int currentIndex, int goalIndex);
    void SetBFSPath(int currentIndex, int goalIndex);
    void SetLineOfSightPath(int currentIndex, int goalIndex);
    void randomMove(int& randomRow, int& randomCol, int goalRow, int goalCol);
    void blockOtherTanks(Tank* tankToExclude);
    void unblockOtherTanks(Tank* tankToExclude);
    void selectPathAlgorithm(int currentIndex, int goalIndex, int currentPlayer);
    void mouseClickToCoords(sf::Vector2f mousePos, int& row, int& col);

public:
    // constructor y destructor
    TankManager(Map* map, Tank** tanks, Player** players, sf::Vector2u windowSize, float cellWidth, float cellHeight);
    ~TankManager();

    // metodos publicos
    void moveTank(sf::Vector2f mousePos, int currentPlayer);
    void AnimateTankMove();
    void TankSelection(sf::Vector2f mousePos, int currentPlayer);

    // verificaciones que Game y BulletController necesitan
    bool isThereATank(int row, int col);
    bool isAnimating();
    bool isReadyToSwitch();

    // getters
    Tank* getActiveTank();
    void clearActiveTank();
};
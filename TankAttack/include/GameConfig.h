#pragma once
// ARCHIVO PARA LAS CONFIGURACIONES GENERALES DEL JUEGO

// Tamanio del mapa
const int MAP_SIZE = 15;

// tiempo del juego
const float GAME_TIME = 300.f;

// Tamanio del margen de informacion del juego (tamanio de 2 celdas)
const int MARGIN_WIDTH = 2;

// cantidad de nodos del grafo
const int GRAPH_SIZE = MAP_SIZE * MAP_SIZE;

// Numero de muros en el mapa
const int TOTAL_WALLS = 30;

// Maximo radio permitido para moverse con los tanques
const int MAX_MOVE_RADIUS = 3;

// Maximo radio en el que se puede realizar un movimiento aleatorio de caso de que no funcione linea vista
const int RANDOM_MOVE_RADIUS = 3;

// velocidad a la que se mueven los tanques (cantidad de pixeles que se mueve)
const float TANK_SPEED = 0.1f;

//velocidad a la que se mueven las balas
const float BULLET_SPEED = 0.1f;

//Rebotes maximos de la bala
const int MAX_BULLET_BOUNCES = 3;

//Cantidad de turnos para un power up
const int TURNS_PER_POWERUP = 4;

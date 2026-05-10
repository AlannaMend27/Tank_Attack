#pragma once
// ARCHIVO PARA LAS CONFIGURACIONES GENERALES DEL JUEGO
// NOTA: TODO EN MAYUSCULA PARA DIFERENCIAR

// Tamanio del mapa
const int MAP_SIZE = 15;

// cantidad de nodos del grafo
const int GRAPH_SIZE = MAP_SIZE * MAP_SIZE;

// Numero de muros en el mapa
const int TOTAL_WALLS = 30;

// Maximo radio permitido para moverse con los tanques
const int MAX_MOVE_RADIUS = 3;

// velocidad a la que se mueven los tanques (cantidad de pixeles que se mueve)
const float TANK_SPEED = 1.0f;

// Tiempo del juego
// const int GAME_TIME = 300;
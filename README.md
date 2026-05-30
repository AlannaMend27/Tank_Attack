# Tank Attack!

Videojuego retro multijugador local por turnos desarrollado en **C++** con **SFML 2.6**.

**Autores:**
- Dylan Bonilla Barquero
- Alanna Mendoza Fonseca

---

## Descripcion

Tank Attack! es un juego de estrategia por turnos para dos jugadores locales. Cada jugador controla dos tanques sobre un mapa generado aleatoriamente, modelado internamente como un grafo con matriz de adyacencia. El objetivo es destruir todos los tanques del oponente, o tener mas tanques activos cuando el tiempo de partida se acabe.

---

## Mecanicas del juego

- **Turnos:** cada jugador puede realizar una accion por turno ya sea mover un tanque, disparar o usar un power-up.
- **Movimiento:** se selecciona un tanque con click izquierdo y se hace click en la celda destino dentro del rango permitido. Los tanques usan distintos algoritmos de pathfinding segun su color:
  - Azul y rosado utilizan BFS o movimiento aleatorio (50/50)
  - Rojo y amarillo utilizan Dijkstra o movimiento aleatorio (80/20)
- **Disparo:** se activa el modo disparo con la tecla `V` y se hace click derecho en el objetivo. Las balas viajan usando linea vista y rebotan en las paredes.
- **Power-ups:** se generan automaticamente cada cierta cantidad de turnos y se consumen con `Shift`. Se aplican en el siguiente turno del jugador:
  - **Doble turno** — el jugador actual tiene dos turnos extra
  - **Precision de movimiento** — fuerza el uso de Dijkstra o BFS segun el color del tanque
  - **Precision de ataque** — la bala sigue el camino mas corto usando A*
  - **Poder de ataque** — la bala elimina el tanque impactado de un golpe
- **Condicion de victoria:**
  - Un jugador pierde todos sus tanques → el otro gana
  - Se acaba el tiempo  → gana quien tenga mas tanques activos
  - Empate si ambos tienen la misma cantidad al terminar el tiempo

---

## 🧠 Algoritmos implementados

Todos los algoritmos y estructuras de datos fueron implementados desde cero, sin usar la STL:

| Algoritmo | Uso |
|---|---|
| **Dijkstra** | Pathfinding de tanques rojo y amarillo |
| **BFS** | Pathfinding de tanques azul y rosado |
| **A\*** | Power-up de precision de ataque |
| **Linea Vista  ** | Trayectoria de balas y movimiento aleatorio |
| **Cola enlazada** | Gestion de power-ups por jugador |
| **Grafo con matriz de adyacencia** | Representacion interna del mapa |

---

## 🗂️ Estructura del proyecto

```
Tank_Attack/
└── TankAttack/
    ├── include/          # Headers (.h) de todas las clases
    │   ├── Game.h
    │   ├── GameConfig.h
    │   ├── Tank.h
    │   ├── Bullet.h
    │   ├── bulletController.h
    │   ├── TankManager.h
    │   ├── UIManager.h
    │   ├── Player.h
    │   ├── PowerUp.h
    │   ├── Map.h
    │   ├── graph.h
    │   ├── AStar.h
    │   ├── BFS.h
    │   ├── dijkstra.h
    │   ├── lineOfSight.h
    │   └── queue.h
    ├── src/              # Implementacion de clases del juego
    │   ├── Main.cpp
    │   ├── Game.cpp
    │   ├── Tank.cpp
    │   ├── Bullet.cpp
    │   ├── BulletControler.cpp
    │   ├── TankManager.cpp
    │   ├── UIManager.cpp
    │   ├── Player.cpp
    │   ├── PowerUp.cpp
    │   └── map.cpp
    ├── structures/       # Algoritmos y estructuras implementadas desde cero
    │   ├── AStar.cpp
    │   ├── BFS.cpp
    │   ├── Dijkstra.cpp
    │   ├── LineOfSight.cpp
    │   ├── graph.cpp
    │   └── Queue.cpp
    ├── assets/           # Recursos graficos y fuentes (SFML)
    ├── lib/              # DLLs de SFML 2.6
    └── TankAttack.vcxproj
```

---

## Requisitos

- **Sistema operativo:** Windows 10 / 11
- **IDE:** Visual Studio (2022-2026)
- **Libreria grafica:** SFML 2.6 (incluida en `lib/`)
- **Arquitectura:** x64

---

##  Como compilar y ejecutar

1. Clonar el repositorio:
```bash
git clone https://github.com/tu-usuario/Tank_Attack.git
```

2. Abrir el archivo `TankAttack.sln` con **Visual Studio 2022-2026**.

3. Seleccionar la configuracion **Debug** o **Release** en x64.

4. Compilar con `Ctrl + Shift + B` o desde el menu **Build → Build Solution**.

5. Ejecutar con `F5` o desde `Build → Start Without Debugging`.


---

## 🎛️ Controles

| Accion | Control |
|---|---|
| Seleccionar tanque | Click izquierdo sobre el tanque |
| Mover tanque | Click izquierdo en celda destino |
| Cambiar a modo disparo | `V` con un tanque seleccionado |
| Disparar | Click derecho en el objetivo |
| Usar power-up | `Shift` |
| Salir | `Escape` |

---

## Configuracion del juego

Los parametros del juego se pueden ajustar desde el archivo `include/GameConfig.h` sin modificar la logica del codigo:

```cpp
const int MAP_SIZE = 15;           // Tamanio del mapa
const float GAME_TIME = 300.f;     // Tiempo de partida en segundos
const int TOTAL_WALLS = 30;        // Cantidad de obstaculos
const int MAX_MOVE_RADIUS = 3;     // Radio maximo de movimiento
const float TANK_SPEED = 0.7f;     // Velocidad de animacion de tanques
const float BULLET_SPEED = 0.8f;   // Velocidad de animacion de balas
const int MAX_BULLET_BOUNCES = 3;  // Rebotes maximos por bala
const int TURNS_PER_POWERUP = 4;   // Turnos entre generacion de power-ups
```

---

##  Arquitectura del sistema

El sistema sigue una arquitectura orientada a objetos donde `Game` actua como coordinador central y delega responsabilidades a tres controladores especializados:

- **`TankManager`** — seleccion, movimiento y animacion de tanques
- **`BulletController`** — disparo, animacion de balas, rebotes e impactos
- **`UIManager`** — renderizado de toda la interfaz del juego



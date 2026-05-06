#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    Game game;

    while (game.getWindowOpen()) {

        // update juego
        game.update();

        //render juego
        game.render();
    }

    return 0;
}

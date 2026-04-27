#include <SFML/Graphics.hpp>

int main() {
    // Creamos una ventana de 800x600 con un título
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML 2.6 Funcionando!");

    // Creamos un círculo verde
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Magenta);
    shape.setPosition(300, 200);

    // Bucle principal
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Si se cierra la ventana
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Limpiar, dibujar y mostrar
        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
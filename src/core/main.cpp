#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window", sf::Style::Titlebar || sf::Style::Close);
    

    while (window.isOpen()) {
        sf::Event event;

        //Event polling 
        while (window.pollEvent(event)) {
            switch (event.type){
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape){
                        window.close();
                    }
                    
                    break;
                
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        //Update
        
        //Render
        window.clear();

        sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
        rectangle.setPosition(50.f, 50.f);
        rectangle.setFillColor(sf::Color::White);

        window.draw(rectangle);

        window.display();
    }

    return 0;
}
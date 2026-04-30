#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML en WSL", sf::Style::Titlebar | sf::Style::Titlebar);
    

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
        window.display();
    }

    return 0;
}
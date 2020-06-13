#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow{ {1024, 720}, "SFML Input Test" };
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                std::cout << "Code: " << std::to_string(event.key.code) << '\n';
                std::cout << "ScanCode: " << std::to_string(event.key.scancode) << '\n';
                std::cout << '\n';
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                std::cout << "Code: " << std::to_string(event.key.code) << '\n';
                std::cout << "ScanCode: " << std::to_string(event.key.scancode) << '\n';
                std::cout << '\n';
            }
        }

        window.clear();
        window.display();
    }
}
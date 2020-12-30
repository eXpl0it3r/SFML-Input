#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow{ {1024, 720}, "SFML Input Test" };
    window.setFramerateLimit(15);

    for (auto i = 0; i < sf::Keyboard::ScanCodeCount; ++i)
    {
        std::cout << i << ":\t" << sf::Keyboard::getDescription(static_cast<sf::Keyboard::Scancode>(i)).toAnsiString() << '\n';
    }

    std::cout << '\n';

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
                std::cout << "Key Pressed\n";
                std::cout << "Code:\t\t" << std::to_string(event.key.code) << '\n';
                std::cout << "ScanCode:\t" << std::to_string(event.key.scancode) << '\n';
                std::cout << "Description:\t" << sf::Keyboard::getDescription(event.key.scancode).toAnsiString() << '\n';
                std::cout << '\n';
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                std::cout << "Key Released\n";
                std::cout << "Code:\t\t" << std::to_string(event.key.code) << '\n';
                std::cout << "ScanCode:\t" << std::to_string(event.key.scancode) << '\n';
                std::cout << "Description:\t" << sf::Keyboard::getDescription(event.key.scancode).toAnsiString() << '\n';
                std::cout << '\n';
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                std::cout << "Mouse Button Pressed\n";
                std::cout << "Button:\t" << std::to_string(event.mouseButton.button) << '\n';
                std::cout << '\n';
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                std::cout << "Mouse Button Released\n";
                std::cout << "Button:\t" << std::to_string(event.mouseButton.button) << '\n';
                std::cout << '\n';
            }
        }

        for (auto i = 0; i < sf::Keyboard::ScanCodeCount; ++i)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(i)))
            {
                std::cout << "IsKeyPressed\n";
                std::cout << "Enum Value:\t" << i << '\n';
                std::cout << "Description:\t" << sf::Keyboard::getDescription(static_cast<sf::Keyboard::Scancode>(i)).toAnsiString() << '\n';
                std::cout << '\n';
            }
        }

        window.clear();
        window.display();
    }
}
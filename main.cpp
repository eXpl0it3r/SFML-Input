#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow{ {1024, 720}, "SFML Input Test" };
    window.setFramerateLimit(15);

    for (auto i = 0; i < sf::Keyboard::ScancodeCount; ++i)
    {
        std::cout << i << ":\t" << sf::Keyboard::getDescription(static_cast<sf::Keyboard::Scancode>(i)).toAnsiString() << '\n';
    }

    std::cout << '\n';

    auto font = sf::Font{};
    if (!font.loadFromFile("Tuffy.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    auto keyPressedText = sf::Text{ "Key Pressed\n", font, 13u };
    keyPressedText.setPosition({ 0.f, 0.f });
    auto keyReleasedText = sf::Text{ "Key Released\n", font, 13u };
    keyReleasedText.setPosition({ 250.f, 0.f });
    auto keyPressedCheckText = sf::Text{ "IsKeyPressed sf::Keyboard::Key\n", font, 13u };
    keyPressedCheckText.setPosition({ 500.f, 0.f });
    auto keyPressedScancodeCheckText = sf::Text{ "IsKeyPressed sf::Keyboard::Scancode\n", font, 13u };
    keyPressedScancodeCheckText.setPosition({ 750.f, 0.f });

    auto mouseButtonPressedText = sf::Text{ "Mouse Button Pressed\n", font, 13u };
    mouseButtonPressedText.setPosition({ 0.f, 400.f });
    auto mouseButtonReleasedText = sf::Text{ "Mouse Button Released\n", font, 13u };
    mouseButtonReleasedText.setPosition({ 250.f, 400.f });
    auto mouseButtonPressedCheckText = sf::Text{ "IsButtonPressed\n", font, 13u };
    mouseButtonPressedCheckText.setPosition({ 500.f, 400.f });

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
                auto text = sf::String{ "Key Pressed" };
                text.insert(text.getSize(), "\nCode:\t\t");
                text.insert(text.getSize(), std::to_string(event.key.code));
                text.insert(text.getSize(), "\nScanCode:\t");
                text.insert(text.getSize(), std::to_string(event.key.scancode));
                text.insert(text.getSize(), "\nDescription:\t");
                text.insert(text.getSize(), sf::Keyboard::getDescription(event.key.scancode));
                text.insert(text.getSize(), "\nLocalized:\t");
                text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(event.key.scancode)));
                text.insert(text.getSize(), "\nDelocalized:\t");
                text.insert(text.getSize(), std::to_string(sf::Keyboard::delocalize(event.key.code)));
                text.insert(text.getSize(), "\n\n");

                keyPressedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                auto text = sf::String{ "Key Released" };
                text.insert(text.getSize(), "\nCode:\t\t");
                text.insert(text.getSize(), std::to_string(event.key.code));
                text.insert(text.getSize(), "\nScanCode:\t");
                text.insert(text.getSize(), std::to_string(event.key.scancode));
                text.insert(text.getSize(), "\nDescription:\t");
                text.insert(text.getSize(), sf::Keyboard::getDescription(event.key.scancode));
                text.insert(text.getSize(), "\nLocalized:\t");
                text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(event.key.scancode)));
                text.insert(text.getSize(), "\nDelocalized:\t");
                text.insert(text.getSize(), std::to_string(sf::Keyboard::delocalize(event.key.code)));
                text.insert(text.getSize(), "\n\n");

                keyReleasedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                auto text = sf::String{ "Mouse Button Pressed" };
                text.insert(text.getSize(), "\nButton:\t");
                text.insert(text.getSize(), std::to_string(event.mouseButton.button));
                text.insert(text.getSize(), "\n\n");

                mouseButtonPressedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                auto text = sf::String{ "Mouse Button Released" };
                text.insert(text.getSize(), "\nButton:\t");
                text.insert(text.getSize(), std::to_string(event.mouseButton.button));
                text.insert(text.getSize(), "\n\n");

                mouseButtonReleasedText.setString(text);
                std::cout << text.toAnsiString();
            }
        }

        for (auto i = 0; i < sf::Keyboard::KeyCount; ++i)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
            {
                auto text = sf::String{ "IsKeyPressed sf::Keyboard::Key" };
                text.insert(text.getSize(), "\nCode:\t");
                text.insert(text.getSize(), std::to_string(i));
                text.insert(text.getSize(), "\nDelocalized:\t");
                text.insert(text.getSize(), std::to_string(sf::Keyboard::delocalize(static_cast<sf::Keyboard::Key>(i))));
                text.insert(text.getSize(), "\n\n");

                keyPressedCheckText.setString(text);
                std::cout << text.toAnsiString();
            }
        }

        for (auto i = 0; i < sf::Keyboard::ScancodeCount; ++i)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(i)))
            {
                auto text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
                text.insert(text.getSize(), "\nScanCode:\t");
                text.insert(text.getSize(), std::to_string(i));
                text.insert(text.getSize(), "\nDescription:\t");
                text.insert(text.getSize(), sf::Keyboard::getDescription(static_cast<sf::Keyboard::Scancode>(i)));
                text.insert(text.getSize(), "\nLocalized:\t");
                text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(static_cast<sf::Keyboard::Scancode>(i))));
                text.insert(text.getSize(), "\n\n");

                keyPressedScancodeCheckText.setString(text);
                std::cout << text.toAnsiString();
            }
        }

        for (auto i = 0; i < sf::Mouse::ButtonCount; ++i)
        {
            if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i)))
            {
                auto text = sf::String{ "IsButtonPressed sf::Mouse::Button" };
                text.insert(text.getSize(), "\nButton:\t");
                text.insert(text.getSize(), std::to_string(i));
                text.insert(text.getSize(), "\n\n");

                mouseButtonPressedCheckText.setString(text);
                std::cout << text.toAnsiString();
            }
        }

        window.clear();

        window.draw(keyPressedText);
        window.draw(keyReleasedText);
        window.draw(keyPressedCheckText);
        window.draw(keyPressedScancodeCheckText);

        window.draw(mouseButtonPressedText);
        window.draw(mouseButtonReleasedText);
        window.draw(mouseButtonPressedCheckText);

        window.display();
    }
}
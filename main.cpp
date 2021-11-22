#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow{ {1920, 1080}, "SFML Input Test" };
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
    keyReleasedText.setPosition({ 260.f, 0.f });

    auto keyPressedCheckText1 = sf::Text{ "IsKeyPressed sf::Keyboard::Key\n", font, 13u };
    keyPressedCheckText1.setPosition({ 520.f, 0.f });
    auto keyPressedCheckText2 = sf::Text{ "IsKeyPressed sf::Keyboard::Key\n", font, 13u };
    keyPressedCheckText2.setPosition({ 780.f, 0.f });

    auto keyPressedScancodeCheckText1 = sf::Text{ "IsKeyPressed sf::Keyboard::Scancode\n", font, 13u };
    keyPressedScancodeCheckText1.setPosition({ 1040.f, 0.f });
    auto keyPressedScancodeCheckText2 = sf::Text{ "IsKeyPressed sf::Keyboard::Scancode\n", font, 13u };
    keyPressedScancodeCheckText2.setPosition({ 1300.f, 0.f });
    auto keyPressedScancodeCheckText3 = sf::Text{ "IsKeyPressed sf::Keyboard::Scancode\n", font, 13u };
    keyPressedScancodeCheckText3.setPosition({ 1560.f, 0.f });

    auto mouseButtonPressedText = sf::Text{ "Mouse Button Pressed\n", font, 13u };
    mouseButtonPressedText.setPosition({ 0.f, 400.f });
    auto mouseButtonReleasedText = sf::Text{ "Mouse Button Released\n", font, 13u };
    mouseButtonReleasedText.setPosition({ 0.f, 500.f });
    auto mouseButtonPressedCheckText = sf::Text{ "IsButtonPressed\n", font, 13u };
    mouseButtonPressedCheckText.setPosition({ 0.f, 600.f });

    auto keys = std::map<sf::Keyboard::Key, bool>{};
    for (auto key = sf::Keyboard::Key::Unknown; key < sf::Keyboard::Key::KeyCount; key = static_cast<sf::Keyboard::Key>(static_cast<unsigned int>(key) + 1))
    {
        keys.emplace(key, false);
    }

    auto scancodeKeys = std::map<sf::Keyboard::Scancode, bool>{};
    for (auto scancode = sf::Keyboard::Scancode::ScanUnknown; scancode < sf::Keyboard::Scancode::ScancodeCount; scancode = static_cast<sf::Keyboard::Scancode>(static_cast<unsigned int>(scancode) + 1))
    {
        scancodeKeys.emplace(scancode, false);
    }

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
                text.insert(text.getSize(), "\n\nCode:\t\t");
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
                text.insert(text.getSize(), "\n\nCode:\t\t");
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
                text.insert(text.getSize(), "\n\nButton:\t");
                text.insert(text.getSize(), std::to_string(event.mouseButton.button));
                text.insert(text.getSize(), "\n\n");

                mouseButtonPressedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                auto text = sf::String{ "Mouse Button Released" };
                text.insert(text.getSize(), "\n\nButton:\t");
                text.insert(text.getSize(), std::to_string(event.mouseButton.button));
                text.insert(text.getSize(), "\n\n");

                mouseButtonReleasedText.setString(text);
                std::cout << text.toAnsiString();
            }
        }

        for (auto i = 0; i < sf::Keyboard::KeyCount; ++i)
        {
            keys[static_cast<sf::Keyboard::Key>(i)] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
        }

        auto text = sf::String{ "IsKeyPressed sf::Keyboard::Key" };
        text.insert(text.getSize(), "\n\nCode / Description / Delocalized / Y/N\n");

        for (auto i = 0; i < sf::Keyboard::Apostrophe; ++i)
        {
            auto code = static_cast<sf::Keyboard::Key>(i);
            auto keyPressed = keys[code];

            text.insert(text.getSize(), std::to_string(i) + " / ");
            text.insert(text.getSize(), sf::Keyboard::getDescription(sf::Keyboard::delocalize(code)) + " / ");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::delocalize(code)) + " / ");
            text.insert(text.getSize(), keyPressed ? "Y" : "N");
            text.insert(text.getSize(), "\n");
        }

        keyPressedCheckText1.setString(text);

        text = sf::String{ "\n\nCode / Description / Delocalized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::Apostrophe); i < sf::Keyboard::KeyCount; ++i)
        {
            auto code = static_cast<sf::Keyboard::Key>(i);
            auto keyPressed = keys[code];

            text.insert(text.getSize(), std::to_string(i) + " / ");
            text.insert(text.getSize(), sf::Keyboard::getDescription(sf::Keyboard::delocalize(code)) + " / ");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::delocalize(code)) + " / ");
            text.insert(text.getSize(), keyPressed ? "Y" : "N");
            text.insert(text.getSize(), "\n");
        }

        keyPressedCheckText2.setString(text);

        for (auto i = 0; i < sf::Keyboard::ScancodeCount; ++i)
        {
            scancodeKeys[static_cast<sf::Keyboard::Scancode>(i)] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(i));
        }

        text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
        text.insert(text.getSize(), "\n\nScanCode / Description / Localized / Y/N\n");

        for (auto i = 0; i < sf::Keyboard::ScanComma; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[scancode];

            text.insert(text.getSize(), std::to_string(i) + " / ");
            text.insert(text.getSize(), sf::Keyboard::getDescription(scancode) + " / ");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(scancode)) + " / ");
            text.insert(text.getSize(), scancodeKeyPressed ? "Y" : "N");
            text.insert(text.getSize(), "\n");
        }

        keyPressedScancodeCheckText1.setString(text);

        text = sf::String{ "\n\nScanCode / Description / Localized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::ScanComma); i < sf::Keyboard::ScanNumpad1; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[scancode];

            text.insert(text.getSize(), std::to_string(i) + " / ");
            text.insert(text.getSize(), sf::Keyboard::getDescription(scancode) + " / ");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(scancode)) + " / ");
            text.insert(text.getSize(), scancodeKeyPressed ? "Y" : "N");
            text.insert(text.getSize(), "\n");
        }

        keyPressedScancodeCheckText2.setString(text);

        text = sf::String{ "\n\nScanCode / Description / Localized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::ScanNumpad1); i < sf::Keyboard::ScancodeCount; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[scancode];

            text.insert(text.getSize(), std::to_string(i) + " / ");
            text.insert(text.getSize(), sf::Keyboard::getDescription(scancode) + " / ");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(scancode)) + " / ");
            text.insert(text.getSize(), scancodeKeyPressed ? "Y" : "N");
            text.insert(text.getSize(), "\n");
        }

        keyPressedScancodeCheckText3.setString(text);

        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::ScanEnter))
        {
            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
            text.insert(text.getSize(), "\nScanCode:\t");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::ScanEnter));
            text.insert(text.getSize(), "\nDescription:\t");
            text.insert(text.getSize(), sf::Keyboard::getDescription(sf::Keyboard::ScanEnter));
            text.insert(text.getSize(), "\nLocalized:\t");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(sf::Keyboard::ScanEnter)));
            text.insert(text.getSize(), "\n\n");

            keyPressedScancodeCheckText.setString(text);
            std::cout << text.toAnsiString();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::ScanNumpadEnter))
        {
            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
            text.insert(text.getSize(), "\nScanCode:\t");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::ScanNumpadEnter));
            text.insert(text.getSize(), "\nDescription:\t");
            text.insert(text.getSize(), sf::Keyboard::getDescription(sf::Keyboard::ScanNumpadEnter));
            text.insert(text.getSize(), "\nLocalized:\t");
            text.insert(text.getSize(), std::to_string(sf::Keyboard::localize(sf::Keyboard::ScanNumpadEnter)));
            text.insert(text.getSize(), "\n\n");

            keyPressedScancodeCheckText.setString(text);
            std::cout << text.toAnsiString();
        }*/

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
        window.draw(keyPressedCheckText1);
        window.draw(keyPressedCheckText2);
        window.draw(keyPressedScancodeCheckText1);
        window.draw(keyPressedScancodeCheckText2);
        window.draw(keyPressedScancodeCheckText3);

        window.draw(mouseButtonPressedText);
        window.draw(mouseButtonReleasedText);
        window.draw(mouseButtonPressedCheckText);

        window.display();
    }
}
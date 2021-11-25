#include <array>
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

    constexpr unsigned int textSize = 13u;
    constexpr unsigned int columnSize = 260u;

    auto keyPressedText = sf::Text{ "Key Pressed\n", font, textSize };
    keyPressedText.setPosition({ 0.f, 0.f });
    auto keyReleasedText = sf::Text{ "Key Released\n", font, textSize };
    keyReleasedText.setPosition({ 1 * columnSize, 0.f });

    auto keyPressedCheckText = std::array<sf::Text, 2>{};
    keyPressedCheckText.fill(sf::Text{ "IsKeyPressed sf::Keyboard::Key\n", font, textSize });
    keyPressedCheckText[0].setPosition({ 2 * columnSize, 0.f });
    keyPressedCheckText[1].setPosition({ 3 * columnSize, 0.f });

    auto keyPressedScancodeCheckText = std::array<sf::Text, 3>{};
    keyPressedScancodeCheckText.fill(sf::Text{ "IsKeyPressed sf::Keyboard::Scancode\n", font, textSize });
    keyPressedScancodeCheckText[0].setPosition({ 4 * columnSize, 0.f });
    keyPressedScancodeCheckText[1].setPosition({ 5 * columnSize, 0.f });
    keyPressedScancodeCheckText[2].setPosition({ 6 * columnSize, 0.f });

    auto mouseButtonPressedText = sf::Text{ "Mouse Button Pressed\n", font, textSize };
    mouseButtonPressedText.setPosition({ 0.f, 400.f });
    auto mouseButtonReleasedText = sf::Text{ "Mouse Button Released\n", font, textSize };
    mouseButtonReleasedText.setPosition({ 0.f, 500.f });
    auto mouseButtonPressedCheckText = sf::Text{ "IsButtonPressed\n", font, textSize };
    mouseButtonPressedCheckText.setPosition({ 0.f, 600.f });

    auto keys = std::array<bool, sf::Keyboard::KeyCount>{};
    auto scancodeKeys = std::array<bool, sf::Keyboard::ScancodeCount>{};

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
                text += "\n\nCode:\t\t";
                text += std::to_string(event.key.code);
                text += "\nScanCode:\t";
                text += std::to_string(event.key.scancode);
                text += "\nDescription:\t";
                text += sf::Keyboard::getDescription(event.key.scancode);
                text += "\nLocalized:\t";
                text += std::to_string(sf::Keyboard::localize(event.key.scancode));
                text += "\nDelocalized:\t";
                text += std::to_string(sf::Keyboard::delocalize(event.key.code));
                text += "\n\n";

                keyPressedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                auto text = sf::String{ "Key Released" };
                text += "\n\nCode:\t\t";
                text += std::to_string(event.key.code);
                text += "\nScanCode:\t";
                text += std::to_string(event.key.scancode);
                text += "\nDescription:\t";
                text += sf::Keyboard::getDescription(event.key.scancode);
                text += "\nLocalized:\t";
                text += std::to_string(sf::Keyboard::localize(event.key.scancode));
                text += "\nDelocalized:\t";
                text += std::to_string(sf::Keyboard::delocalize(event.key.code));
                text += "\n\n";

                keyReleasedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                auto text = sf::String{ "Mouse Button Pressed" };
                text += "\n\nButton:\t";
                text += std::to_string(event.mouseButton.button);
                text += "\n\n";

                mouseButtonPressedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                auto text = sf::String{ "Mouse Button Released" };
                text += "\n\nButton:\t";
                text += std::to_string(event.mouseButton.button);
                text += "\n\n";

                mouseButtonReleasedText.setString(text);
                std::cout << text.toAnsiString();
            }
        }

        for (auto i = 0; i < sf::Keyboard::KeyCount; ++i)
        {
            keys[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
        }

        auto text = sf::String{ "IsKeyPressed sf::Keyboard::Key" };
        text += "\n\nCode / Description / Delocalized / Y/N\n";

        for (auto i = 0; i < sf::Keyboard::Apostrophe; ++i)
        {
            auto code = static_cast<sf::Keyboard::Key>(i);
            auto keyPressed = keys[i];

            text += std::to_string(i) + " / ";
            text += sf::Keyboard::getDescription(sf::Keyboard::delocalize(code)) + " / ";
            text += std::to_string(sf::Keyboard::delocalize(code)) + " / ";
            text += keyPressed ? "Y" : "N";
            text += "\n";
        }

        keyPressedCheckText[0].setString(text);

        text = sf::String{ "\n\nCode / Description / Delocalized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::Apostrophe); i < sf::Keyboard::KeyCount; ++i)
        {
            auto code = static_cast<sf::Keyboard::Key>(i);
            auto keyPressed = keys[i];

            text += std::to_string(i) + " / ";
            text += sf::Keyboard::getDescription(sf::Keyboard::delocalize(code)) + " / ";
            text += std::to_string(sf::Keyboard::delocalize(code)) + " / ";
            text += keyPressed ? "Y" : "N";
            text += "\n";
        }

        keyPressedCheckText[1].setString(text);

        for (auto i = 0; i < sf::Keyboard::ScancodeCount; ++i)
        {
            scancodeKeys[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(i));
        }

        text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
        text += "\n\nScanCode / Description / Localized / Y/N\n";

        for (auto i = 0; i < sf::Keyboard::ScanComma; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[i];

            text += std::to_string(i) + " / ";
            text += sf::Keyboard::getDescription(scancode) + " / ";
            text += std::to_string(sf::Keyboard::localize(scancode)) + " / ";
            text += scancodeKeyPressed ? "Y" : "N";
            text += "\n";
        }

        keyPressedScancodeCheckText[0].setString(text);

        text = sf::String{ "\n\nScanCode / Description / Localized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::ScanComma); i < sf::Keyboard::ScanNumpad1; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[i];

            text += std::to_string(i) + " / ";
            text += sf::Keyboard::getDescription(scancode) + " / ";
            text += std::to_string(sf::Keyboard::localize(scancode)) + " / ";
            text += scancodeKeyPressed ? "Y" : "N";
            text += "\n";
        }

        keyPressedScancodeCheckText[1].setString(text);

        text = sf::String{ "\n\nScanCode / Description / Localized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::ScanNumpad1); i < sf::Keyboard::ScancodeCount; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[i];

            text += std::to_string(i) + " / ";
            text += sf::Keyboard::getDescription(scancode) + " / ";
            text += std::to_string(sf::Keyboard::localize(scancode)) + " / ";
            text += scancodeKeyPressed ? "Y" : "N";
            text += "\n";
        }

        keyPressedScancodeCheckText[2].setString(text);

        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::ScanEnter))
        {
            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
            text += "\nScanCode:\t";
            text += std::to_string(sf::Keyboard::ScanEnter);
            text += "\nDescription:\t";
            text += sf::Keyboard::getDescription(sf::Keyboard::ScanEnter);
            text += "\nLocalized:\t";
            text += std::to_string(sf::Keyboard::localize(sf::Keyboard::ScanEnter));
            text += "\n\n";

            keyPressedScancodeCheckText.setString(text);
            std::cout << text.toAnsiString();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::ScanNumpadEnter))
        {
            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
            text += "\nScanCode:\t";
            text += std::to_string(sf::Keyboard::ScanNumpadEnter);
            text += "\nDescription:\t";
            text += sf::Keyboard::getDescription(sf::Keyboard::ScanNumpadEnter);
            text += "\nLocalized:\t";
            text += std::to_string(sf::Keyboard::localize(sf::Keyboard::ScanNumpadEnter));
            text += "\n\n";

            keyPressedScancodeCheckText.setString(text);
            std::cout << text.toAnsiString();
        }*/

        for (auto i = 0; i < sf::Mouse::ButtonCount; ++i)
        {
            if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i)))
            {
                auto text = sf::String{ "IsButtonPressed sf::Mouse::Button" };
                text += "\nButton:\t";
                text += std::to_string(i);
                text += "\n\n";

                mouseButtonPressedCheckText.setString(text);
                std::cout << text.toAnsiString();
            }
        }

        window.clear();

        window.draw(keyPressedText);
        window.draw(keyReleasedText);

        for (const auto& text : keyPressedCheckText)
            window.draw(text);
        for (const auto& text : keyPressedScancodeCheckText)
            window.draw(text);

        window.draw(mouseButtonPressedText);
        window.draw(mouseButtonReleasedText);
        window.draw(mouseButtonPressedCheckText);

        window.display();
    }
}
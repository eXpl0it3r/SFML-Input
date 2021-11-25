#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>

sf::String keyEventDescription(sf::String text, const sf::Event::KeyEvent& keyEvent)
{
    text += "\n\nCode:\t\t";
    text += std::to_string(keyEvent.code);
    text += "\nScanCode:\t";
    text += std::to_string(keyEvent.scancode);
    text += "\nDescription:\t";
    text += sf::Keyboard::getDescription(keyEvent.scancode);
    text += "\nLocalized:\t";
    text += std::to_string(sf::Keyboard::localize(keyEvent.scancode));
    text += "\nDelocalized:\t";
    text += std::to_string(sf::Keyboard::delocalize(keyEvent.code));
    text += "\n\n";

    return text;
}

sf::String buttonDescription(sf::String text, sf::Mouse::Button button)
{
    text += "\n\nButton:\t";
    text += std::to_string(button);
    text += "\n\n";

    return text;
}

sf::String keyDescription(sf::Keyboard::Key code, bool keyPressed)
{
    sf::String text = std::to_string(code) + " / ";
    text += sf::Keyboard::getDescription(sf::Keyboard::delocalize(code)) + " / ";
    text += std::to_string(sf::Keyboard::delocalize(code)) + " / ";
    text += keyPressed ? "Y" : "N";
    text += "\n";

    return text;
}

sf::String scancodeDescription(sf::Keyboard::Scancode scancode, bool scancodeKeyPressed)
{
    sf::String text = std::to_string(scancode) + " / ";
    text += sf::Keyboard::getDescription(scancode) + " / ";
    text += std::to_string(sf::Keyboard::localize(scancode)) + " / ";
    text += scancodeKeyPressed ? "Y" : "N";
    text += "\n";

    return text;
}

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
                auto text = keyEventDescription("Key Pressed", event.key);

                keyPressedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                auto text = keyEventDescription("Key Released", event.key);

                keyReleasedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                auto text = buttonDescription("Mouse Button Pressed", event.mouseButton.button);

                mouseButtonPressedText.setString(text);
                std::cout << text.toAnsiString();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                auto text = buttonDescription("Mouse Button Released", event.mouseButton.button);

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

            text += keyDescription(code, keyPressed);
        }

        keyPressedCheckText[0].setString(text);

        text = sf::String{ "\n\nCode / Description / Delocalized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::Apostrophe); i < sf::Keyboard::KeyCount; ++i)
        {
            auto code = static_cast<sf::Keyboard::Key>(i);
            auto keyPressed = keys[i];

            text += keyDescription(code, keyPressed);
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

            text += scancodeDescription(scancode, scancodeKeyPressed);
        }

        keyPressedScancodeCheckText[0].setString(text);

        text = sf::String{ "\n\nScanCode / Description / Localized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::ScanComma); i < sf::Keyboard::ScanNumpad1; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[i];

            text += scancodeDescription(scancode, scancodeKeyPressed);
        }

        keyPressedScancodeCheckText[1].setString(text);

        text = sf::String{ "\n\nScanCode / Description / Localized / Y/N\n" };

        for (auto i = static_cast<int>(sf::Keyboard::ScanNumpad1); i < sf::Keyboard::ScancodeCount; ++i)
        {
            auto scancode = static_cast<sf::Keyboard::Scancode>(i);
            auto scancodeKeyPressed = scancodeKeys[i];

            text += scancodeDescription(scancode, scancodeKeyPressed);
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
            auto button = static_cast<sf::Mouse::Button>(i);
            if (sf::Mouse::isButtonPressed(button))
            {
                auto text = buttonDescription("IsButtonPressed sf::Mouse::Button", button);

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
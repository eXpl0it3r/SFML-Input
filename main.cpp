#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

std::string encodeStringToAnsi(const sf::String& string);
std::string encodeStringToUtf8(const sf::String& string);

sf::String keyEventDescription(sf::String init, const sf::Event::KeyEvent& keyEvent);
sf::String buttonEventDescription(sf::String init, const sf::Event::MouseButtonEvent& buttonEvent);

sf::String keyDescription(sf::Keyboard::Key code, bool keyPressed);
sf::String scancodeDescription(sf::Keyboard::Scancode scancode, bool scancodeKeyPressed);
sf::String buttonDescription(sf::Mouse::Button button, bool buttonPressed);

bool seemsStrange(const sf::Event::KeyEvent& keyEvent);

sf::String keyIdentifier(sf::Keyboard::Key code);
sf::String scancodeIdentifier(sf::Keyboard::Scancode scancode);
sf::String buttonIdentifier(sf::Mouse::Button button);

int main(int argc, char* argv[])
{
    // Select UTF-8 output if the program was launched with an argument
    const auto encode = argc > 1 ? encodeStringToUtf8 : encodeStringToAnsi;

    auto window = sf::RenderWindow{ {1920, 1080}, "SFML Input Test" };
    window.setFramerateLimit(15);

    for (auto i = 0; i < sf::Keyboard::ScancodeCount; ++i)
    {
        std::cout << i << ":\t" << encode(sf::Keyboard::getDescription(static_cast<sf::Keyboard::Scancode>(i))) << '\n';
    }

    std::cout << '\n';

    sf::SoundBuffer errorSoundBuffer, pressedSoundBuffer, releasedSoundBuffer;
    if (!errorSoundBuffer.loadFromFile("error_005.ogg") ||
        !pressedSoundBuffer.loadFromFile("mouseclick1.ogg") ||
        !releasedSoundBuffer.loadFromFile("mouserelease1.ogg"))
    {
        std::cerr << "Failed to load sound" << std::endl;
        return 1;
    }

    auto errorSound    = sf::Sound{ errorSoundBuffer },
         pressedSound  = sf::Sound{ pressedSoundBuffer },
         releasedSound = sf::Sound{ releasedSoundBuffer };

    auto font = sf::Font{};
    if (!font.loadFromFile("Tuffy.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    constexpr unsigned int textSize = 14u;
    constexpr unsigned int columnSize = 300u;

    auto keyPressedText = sf::Text{ "Key Pressed\n", font, textSize };
    keyPressedText.setPosition({ 0.f, 0.f });
    auto keyReleasedText = sf::Text{ "Key Released\n", font, textSize };
    keyReleasedText.setPosition({ 0.f, 200.f });

    auto keyPressedCheckText = std::array<sf::Text, 2>{};
    keyPressedCheckText.fill(sf::Text{ "IsKeyPressed sf::Keyboard::Key\n", font, textSize });
    keyPressedCheckText[0].setPosition({ 1 * columnSize, 0.f });
    keyPressedCheckText[1].setPosition({ 2 * columnSize, 0.f });

    auto keyPressedScancodeCheckText = std::array<sf::Text, 3>{};
    keyPressedScancodeCheckText.fill(sf::Text{ "IsKeyPressed sf::Keyboard::Scancode\n", font, textSize });
    keyPressedScancodeCheckText[0].setPosition({ 3 * columnSize, 0.f });
    keyPressedScancodeCheckText[1].setPosition({ 4 * columnSize, 0.f });
    keyPressedScancodeCheckText[2].setPosition({ 5 * columnSize, 0.f });

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
                std::cout << encode(text);

                if(seemsStrange(event.key))
                    errorSound.play();
                else
                    pressedSound.play();
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                auto text = keyEventDescription("Key Released", event.key);

                keyReleasedText.setString(text);
                std::cout << encode(text);

                if(seemsStrange(event.key))
                    errorSound.play();
                else
                    releasedSound.play();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                auto text = buttonEventDescription("Mouse Button Pressed", event.mouseButton);

                mouseButtonPressedText.setString(text);
                std::cout << encode(text);

                pressedSound.play();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                auto text = buttonEventDescription("Mouse Button Released", event.mouseButton);

                mouseButtonReleasedText.setString(text);
                std::cout << encode(text);

                releasedSound.play();
            }
        }

        for (auto i = 0u; i < sf::Keyboard::KeyCount; ++i)
        {
            keys[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
        }

        {
            constexpr auto keyBounds = std::array<unsigned, 3>{0, sf::Keyboard::Apostrophe, sf::Keyboard::KeyCount};
            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Key" };
            for (auto b = 0u; b < keyBounds.size() - 1; ++b)
            {
                text += "\n\nCode / Description / Delocalized / Pressed\n";
                for (auto i = keyBounds[b]; i < keyBounds[b + 1]; ++i)
                {
                    auto code = static_cast<sf::Keyboard::Key>(i);
                    auto keyPressed = keys[i];

                    text += keyDescription(code, keyPressed);
                }

                keyPressedCheckText[b].setString(text);
                text.clear();
            }
        }

        for (auto i = 0u; i < sf::Keyboard::ScancodeCount; ++i)
        {
            scancodeKeys[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(i));
        }

        {
            constexpr auto scancodeBounds = std::array<unsigned, 4>{0, sf::Keyboard::ScanComma, sf::Keyboard::ScanNumpad1, sf::Keyboard::ScancodeCount};
            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
            for (auto b = 0u; b < scancodeBounds.size() - 1; ++b)
            {
                text += "\n\nScanCode / Description / Localized / Pressed\n";
                for (auto i = scancodeBounds[b]; i < scancodeBounds[b + 1]; ++i)
                {
                    auto scancode = static_cast<sf::Keyboard::Scancode>(i);
                    auto scancodeKeyPressed = scancodeKeys[i];

                    text += scancodeDescription(scancode, scancodeKeyPressed);
                }

                keyPressedScancodeCheckText[b].setString(text);
                text.clear();
            }
        }

        {
            auto text = sf::String{ "IsButtonPressed sf::Mouse::Button\n\n" };
            for (auto i = 0u; i < sf::Mouse::ButtonCount; ++i)
            {
                auto button = static_cast<sf::Mouse::Button>(i);
                auto buttonPressed = sf::Mouse::isButtonPressed(button);

                text += buttonDescription(button, buttonPressed);
            }

            mouseButtonPressedCheckText.setString(text);
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

std::string encodeStringToAnsi(const sf::String& string)
{
    return string.toAnsiString();
}

std::string encodeStringToUtf8(const sf::String& string)
{
    // sf::String::toUtf8 returns std::basic_string<sf::Uint8>
    // which cannot be converted cheaply to std::basic_string<char>
    // so the conversion is done with sf::Utf32::toUtf8 here.

    std::string output;
    output.reserve(string.getSize());

    sf::Utf32::toUtf8(string.begin(), string.end(), std::back_inserter(output));

    return output;
}

sf::String keyEventDescription(sf::String text, const sf::Event::KeyEvent& keyEvent)
{
    text += "\n\nCode:\t\t";
    text += std::to_string(keyEvent.code);
    text += "\tsf::Keyboard::";
    text += keyIdentifier(keyEvent.code);
    text += "\nScanCode:\t";
    text += std::to_string(keyEvent.scancode);
    text += "\tsf::Keyboard::";
    text += scancodeIdentifier(keyEvent.scancode);
    text += "\nDescription:\t";
    text += sf::Keyboard::getDescription(keyEvent.scancode);
    text += "\nLocalized:\t";
    text += std::to_string(sf::Keyboard::localize(keyEvent.scancode));
    text += "\tsf::Keyboard::";
    text += keyIdentifier(sf::Keyboard::localize(keyEvent.scancode));
    text += "\nDelocalized:\t";
    text += std::to_string(sf::Keyboard::delocalize(keyEvent.code));
    text += "\tsf::Keyboard::";
    text += scancodeIdentifier(sf::Keyboard::delocalize(keyEvent.code));
    text += "\n\n";

    return text;
}

sf::String buttonEventDescription(sf::String text, const sf::Event::MouseButtonEvent& buttonEvent)
{
    text += "\n\nButton:\t";
    text += std::to_string(buttonEvent.button);
    text += "\tsf::Mouse::";
    text += buttonIdentifier(buttonEvent.button);
    text += "\n\n";

    return text;
}

sf::String keyDescription(sf::Keyboard::Key code, bool keyPressed)
{
    sf::String text = std::to_string(code) + " / ";
    text += sf::Keyboard::getDescription(sf::Keyboard::delocalize(code)) + " / ";
    text += std::to_string(sf::Keyboard::delocalize(code));
    text += keyPressed ? " Pressed" : "";
    text += "\n";

    return text;
}

sf::String scancodeDescription(sf::Keyboard::Scancode scancode, bool scancodeKeyPressed)
{
    sf::String text = std::to_string(scancode) + " / ";
    text += sf::Keyboard::getDescription(scancode) + " / ";
    text += std::to_string(sf::Keyboard::localize(scancode));
    text += scancodeKeyPressed ? " Pressed" : "";
    text += "\n";

    return text;
}

sf::String buttonDescription(sf::Mouse::Button button, bool buttonPressed)
{
    sf::String text = std::to_string(button) + " / ";
    text += "sf::Mouse::";
    text += buttonIdentifier(button);
    text += buttonPressed ? "\tPressed" : "";
    text += "\n";

    return text;
}

bool seemsStrange(const sf::Event::KeyEvent& keyEvent)
{
    return keyEvent.code == -1
        || keyEvent.scancode == -1
        || sf::Keyboard::getDescription(keyEvent.scancode) == ""
        || sf::Keyboard::localize(keyEvent.scancode) != keyEvent.code
        || sf::Keyboard::delocalize(keyEvent.code) != keyEvent.scancode;
}

sf::String keyIdentifier(sf::Keyboard::Key code)
{
    static constexpr auto identifiers = std::array
    {
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
        "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Num0", "Num1",
        "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9", "Escape",
        "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt",
        "RSystem", "Menu", "LBracket", "RBracket", "Semicolon", "Comma", "Period",
        "Apostrophe", "Slash", "Backslash", "Tilde", "Equal", "Hyphen", "Space",
        "Enter", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert",
        "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right", "Up",
        "Down", "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5",
        "Numpad6", "Numpad7", "Numpad8", "Numpad9", "F1", "F2", "F3", "F4", "F5",
        "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "Pause"
         };

    return code == -1 ? "Unknown" : identifiers[code];
}

sf::String scancodeIdentifier(sf::Keyboard::Scancode scancode)
{
    static constexpr auto identifiers = std::array
    {
        "ScanA", "ScanB", "ScanC", "ScanD", "ScanE", "ScanF", "ScanG", "ScanH", "ScanI",
        "ScanJ", "ScanK", "ScanL", "ScanM", "ScanN", "ScanO", "ScanP", "ScanQ",
        "ScanR", "ScanS", "ScanT", "ScanU", "ScanV", "ScanW", "ScanX", "ScanY",
        "ScanZ", "ScanNum1", "ScanNum2", "ScanNum3", "ScanNum4", "ScanNum5", "ScanNum6",
        "ScanNum7", "ScanNum8", "ScanNum9", "ScanNum0", "ScanEnter", "ScanEscape",
        "ScanBackspace", "ScanTab", "ScanSpace", "ScanHyphen", "ScanEqual", "ScanLBracket",
        "ScanRBracket", "ScanBackslash", "ScanSemicolon", "ScanApostrophe", "ScanGrave",
        "ScanComma", "ScanPeriod", "ScanSlash", "ScanF1", "ScanF2", "ScanF3", "ScanF4",
        "ScanF5", "ScanF6", "ScanF7", "ScanF8", "ScanF9", "ScanF10", "ScanF11",
        "ScanF12", "ScanF13", "ScanF14", "ScanF15", "ScanF16", "ScanF17", "ScanF18",
        "ScanF19", "ScanF20", "ScanF21", "ScanF22", "ScanF23", "ScanF24", "ScanCapsLock",
        "ScanPrintScreen", "ScanScrollLock", "ScanPause", "ScanInsert", "ScanHome",
        "ScanPageUp", "ScanDelete", "ScanEnd", "ScanPageDown", "ScanRight", "ScanLeft",
        "ScanDown", "ScanUp", "ScanNumLock", "ScanNumpadDivide", "ScanNumpadMultiply",
        "ScanNumpadMinus", "ScanNumpadPlus", "ScanNumpadEqual", "ScanNumpadEnter",
        "ScanNumpadDecimal", "ScanNumpad1", "ScanNumpad2", "ScanNumpad3", "ScanNumpad4",
        "ScanNumpad5", "ScanNumpad6", "ScanNumpad7", "ScanNumpad8", "ScanNumpad9",
        "ScanNumpad0", "ScanNonUsBackslash", "ScanApplication", "ScanExecute",
        "ScanModeChange", "ScanHelp", "ScanMenu", "ScanSelect", "ScanRedo", "ScanUndo",
        "ScanCut", "ScanCopy", "ScanPaste", "ScanVolumeMute", "ScanVolumeUp", "ScanVolumeDown",
        "ScanMediaPlayPause", "ScanMediaStop", "ScanMediaNextTrack", "ScanMediaPreviousTrack",
        "ScanLControl", "ScanLShift", "ScanLAlt", "ScanLSystem", "ScanRControl",
        "ScanRShift", "ScanRAlt", "ScanRSystem", "ScanBack", "ScanForward", "ScanRefresh",
        "ScanStop", "ScanSearch", "ScanFavorites", "ScanHomePage", "ScanLaunchApplication1",
        "ScanLaunchApplication2", "ScanLaunchMail", "ScanLaunchMediaSelect"
     };

    return scancode == -1 ? "ScanUnknown" : identifiers[scancode];
}

sf::String buttonIdentifier(sf::Mouse::Button button)
{
    static constexpr auto identifiers = std::array
    {
        "Left", "Right", "Middle", "XButton1", "XButton2"
    };

    return identifiers[button];
}

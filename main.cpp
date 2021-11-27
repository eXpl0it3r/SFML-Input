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

std::string keyIdentifier(sf::Keyboard::Key code);
std::string scancodeIdentifier(sf::Keyboard::Scancode scancode);
std::string buttonIdentifier(sf::Mouse::Button button);

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

std::string keyIdentifier(sf::Keyboard::Key code)
{
    // Inspired by mantognini/SFML-Test-Events

    static_assert(sf::Keyboard::KeyCount == 101, "Number of SFML keys has changed. The switch statement must be updated.");

    switch(code)
    {
        #define CASE(code) case sf::Keyboard::code: return #code
        CASE(Unknown);
        CASE(A);
        CASE(B);
        CASE(C);
        CASE(D);
        CASE(E);
        CASE(F);
        CASE(G);
        CASE(H);
        CASE(I);
        CASE(J);
        CASE(K);
        CASE(L);
        CASE(M);
        CASE(N);
        CASE(O);
        CASE(P);
        CASE(Q);
        CASE(R);
        CASE(S);
        CASE(T);
        CASE(U);
        CASE(V);
        CASE(W);
        CASE(X);
        CASE(Y);
        CASE(Z);
        CASE(Num0);
        CASE(Num1);
        CASE(Num2);
        CASE(Num3);
        CASE(Num4);
        CASE(Num5);
        CASE(Num6);
        CASE(Num7);
        CASE(Num8);
        CASE(Num9);
        CASE(Escape);
        CASE(LControl);
        CASE(LShift);
        CASE(LAlt);
        CASE(LSystem);
        CASE(RControl);
        CASE(RShift);
        CASE(RAlt);
        CASE(RSystem);
        CASE(Menu);
        CASE(LBracket);
        CASE(RBracket);
        CASE(Semicolon);
        CASE(Comma);
        CASE(Period);
        CASE(Apostrophe);
        CASE(Slash);
        CASE(Backslash);
        CASE(Tilde);
        CASE(Equal);
        CASE(Hyphen);
        CASE(Space);
        CASE(Enter);
        CASE(Backspace);
        CASE(Tab);
        CASE(PageUp);
        CASE(PageDown);
        CASE(End);
        CASE(Home);
        CASE(Insert);
        CASE(Delete);
        CASE(Add);
        CASE(Subtract);
        CASE(Multiply);
        CASE(Divide);
        CASE(Left);
        CASE(Right);
        CASE(Up);
        CASE(Down);
        CASE(Numpad0);
        CASE(Numpad1);
        CASE(Numpad2);
        CASE(Numpad3);
        CASE(Numpad4);
        CASE(Numpad5);
        CASE(Numpad6);
        CASE(Numpad7);
        CASE(Numpad8);
        CASE(Numpad9);
        CASE(F1);
        CASE(F2);
        CASE(F3);
        CASE(F4);
        CASE(F5);
        CASE(F6);
        CASE(F7);
        CASE(F8);
        CASE(F9);
        CASE(F10);
        CASE(F11);
        CASE(F12);
        CASE(F13);
        CASE(F14);
        CASE(F15);
        CASE(Pause);
        #undef CASE

        case sf::Keyboard::KeyCount: // So that all values are handled.
            throw std::runtime_error{ "invalid keyboard code" };

        // No default case on purpose so that compilers can reports unhandled values.
    }

    throw std::runtime_error{ "invalid keyboard code" };
}

std::string scancodeIdentifier(sf::Keyboard::Scancode scancode)
{
    // Same design as the keyIdentifier function

    static_assert(sf::Keyboard::ScancodeCount == 146, "Number of SFML scancodes has changed. The switch statement must be updated.");

    switch(scancode)
    {
        #define CASE(scancode) case sf::Keyboard::scancode: return #scancode
        CASE(ScanUnknown);
        CASE(ScanA);
        CASE(ScanB);
        CASE(ScanC);
        CASE(ScanD);
        CASE(ScanE);
        CASE(ScanF);
        CASE(ScanG);
        CASE(ScanH);
        CASE(ScanI);
        CASE(ScanJ);
        CASE(ScanK);
        CASE(ScanL);
        CASE(ScanM);
        CASE(ScanN);
        CASE(ScanO);
        CASE(ScanP);
        CASE(ScanQ);
        CASE(ScanR);
        CASE(ScanS);
        CASE(ScanT);
        CASE(ScanU);
        CASE(ScanV);
        CASE(ScanW);
        CASE(ScanX);
        CASE(ScanY);
        CASE(ScanZ);
        CASE(ScanNum1);
        CASE(ScanNum2);
        CASE(ScanNum3);
        CASE(ScanNum4);
        CASE(ScanNum5);
        CASE(ScanNum6);
        CASE(ScanNum7);
        CASE(ScanNum8);
        CASE(ScanNum9);
        CASE(ScanNum0);
        CASE(ScanEnter);
        CASE(ScanEscape);
        CASE(ScanBackspace);
        CASE(ScanTab);
        CASE(ScanSpace);
        CASE(ScanHyphen);
        CASE(ScanEqual);
        CASE(ScanLBracket);
        CASE(ScanRBracket);
        CASE(ScanBackslash);
        CASE(ScanSemicolon);
        CASE(ScanApostrophe);
        CASE(ScanGrave);
        CASE(ScanComma);
        CASE(ScanPeriod);
        CASE(ScanSlash);
        CASE(ScanF1);
        CASE(ScanF2);
        CASE(ScanF3);
        CASE(ScanF4);
        CASE(ScanF5);
        CASE(ScanF6);
        CASE(ScanF7);
        CASE(ScanF8);
        CASE(ScanF9);
        CASE(ScanF10);
        CASE(ScanF11);
        CASE(ScanF12);
        CASE(ScanF13);
        CASE(ScanF14);
        CASE(ScanF15);
        CASE(ScanF16);
        CASE(ScanF17);
        CASE(ScanF18);
        CASE(ScanF19);
        CASE(ScanF20);
        CASE(ScanF21);
        CASE(ScanF22);
        CASE(ScanF23);
        CASE(ScanF24);
        CASE(ScanCapsLock);
        CASE(ScanPrintScreen);
        CASE(ScanScrollLock);
        CASE(ScanPause);
        CASE(ScanInsert);
        CASE(ScanHome);
        CASE(ScanPageUp);
        CASE(ScanDelete);
        CASE(ScanEnd);
        CASE(ScanPageDown);
        CASE(ScanRight);
        CASE(ScanLeft);
        CASE(ScanDown);
        CASE(ScanUp);
        CASE(ScanNumLock);
        CASE(ScanNumpadDivide);
        CASE(ScanNumpadMultiply);
        CASE(ScanNumpadMinus);
        CASE(ScanNumpadPlus);
        CASE(ScanNumpadEqual);
        CASE(ScanNumpadEnter);
        CASE(ScanNumpadDecimal);
        CASE(ScanNumpad1);
        CASE(ScanNumpad2);
        CASE(ScanNumpad3);
        CASE(ScanNumpad4);
        CASE(ScanNumpad5);
        CASE(ScanNumpad6);
        CASE(ScanNumpad7);
        CASE(ScanNumpad8);
        CASE(ScanNumpad9);
        CASE(ScanNumpad0);
        CASE(ScanNonUsBackslash);
        CASE(ScanApplication);
        CASE(ScanExecute);
        CASE(ScanModeChange);
        CASE(ScanHelp);
        CASE(ScanMenu);
        CASE(ScanSelect);
        CASE(ScanRedo);
        CASE(ScanUndo);
        CASE(ScanCut);
        CASE(ScanCopy);
        CASE(ScanPaste);
        CASE(ScanVolumeMute);
        CASE(ScanVolumeUp);
        CASE(ScanVolumeDown);
        CASE(ScanMediaPlayPause);
        CASE(ScanMediaStop);
        CASE(ScanMediaNextTrack);
        CASE(ScanMediaPreviousTrack);
        CASE(ScanLControl);
        CASE(ScanLShift);
        CASE(ScanLAlt);
        CASE(ScanLSystem);
        CASE(ScanRControl);
        CASE(ScanRShift);
        CASE(ScanRAlt);
        CASE(ScanRSystem);
        CASE(ScanBack);
        CASE(ScanForward);
        CASE(ScanRefresh);
        CASE(ScanStop);
        CASE(ScanSearch);
        CASE(ScanFavorites);
        CASE(ScanHomePage);
        CASE(ScanLaunchApplication1);
        CASE(ScanLaunchApplication2);
        CASE(ScanLaunchMail);
        CASE(ScanLaunchMediaSelect);
        #undef CASE

        case sf::Keyboard::ScancodeCount:
            throw std::runtime_error{ "invalid keyboard scancode" };
    }

    throw std::runtime_error{ "invalid keyboard scancode" };
}

std::string buttonIdentifier(sf::Mouse::Button button)
{
    // Same design as the keyIdentifier function

    static_assert(sf::Mouse::ButtonCount == 5, "Number of SFML mouse buttons has changed. The switch statement must be updated.");

    switch(button)
    {
        #define CASE(button) case sf::Mouse::button: return #button
        CASE(Left);
        CASE(Right);
        CASE(Middle);
        CASE(XButton1);
        CASE(XButton2);
        #undef CASE

        case sf::Mouse::ButtonCount:
            throw std::runtime_error{ "invalid mouse button" };
    }

    throw std::runtime_error{ "invalid mouse button" };
}

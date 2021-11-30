#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

constexpr auto help =
"  -v, --verbose   Show more information\n"
"  -d, --dot       Generate dot diagram about localize and delocalize functions\n"
"  -u, --utf8      Encode console output as UTF-8 instead of ANSI\n"
"  -h, --help      Show help and exit";

struct Arguments
{
    Arguments(int argc, char* argv[]);

    bool verbose = false;
    bool generateDiagram = false;
    bool utf8 = false;
    bool help = false;
};

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

std::ostream& operator<<(std::ostream& os, sf::Keyboard::Key code);
std::ostream& operator<<(std::ostream& os, sf::Keyboard::Scancode scancode);

template <typename Enum, typename Function>
void forEachEnum(Enum begin, Enum end, Function function)
{
    for (int i = begin; i < end; ++i)
        function(static_cast<Enum>(i));
}

template <typename Function>
void forEachKey(Function function)
{
    forEachEnum(static_cast<sf::Keyboard::Key>(0), sf::Keyboard::KeyCount, function);
}

template <typename Function>
void forEachScancode(Function function)
{
    forEachEnum(static_cast<sf::Keyboard::Scancode>(0), sf::Keyboard::ScancodeCount, function);
}

int main(int argc, char* argv[])
{
    const auto args = Arguments{ argc, argv };
    if(args.help)
    {
        std::cout << "Usage: " << argv[0] << " [OPTION]...\n\n"
                  << help << '\n';

        return 0;
    }

    const auto encode = args.utf8 ? encodeStringToUtf8 : encodeStringToAnsi;

    auto window = sf::RenderWindow{ {1920, 1080}, "SFML Input Test" };
    window.setFramerateLimit(15);

    std::cout << "\tScancode descriptions\n\n";
    forEachScancode([encode](auto scancode)
    {
        std::cout << std::right << std::setw(3) << static_cast<int>(scancode) << ' '
                  << std::left  << std::setw(22) << scancode << ' '
                  << encode(sf::Keyboard::getDescription(scancode)) << '\n';
    });
    std::cout << '\n';

    // Output information about localize and delocalize
    if(args.verbose)
    {
        std::cout << "\tKeys for which delocalize(key) == ScanUnknown\n\n";
        forEachKey([](auto key)
        {
            if(auto scancode = sf::Keyboard::delocalize(key); scancode == sf::Keyboard::ScanUnknown)
                std::cout << std::setw(10) << key << " -> " << scancode << '\n';
        });
        std::cout << '\n';

        std::cout << "\tOther keys for which localize(delocalize(key)) == Unknown\n\n";
        forEachKey([](auto key)
        {
            if(auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::ScanUnknown)
                if(auto key2 = sf::Keyboard::localize(scancode); key2 == sf::Keyboard::Unknown)
                    std::cout << std::setw(10) << key << " -> " << std::setw(22) << scancode << " -> " << key2 << '\n';
        });
        std::cout << '\n';

        std::cout << "\tOther keys for which localize(delocalize(key)) != key\n\n";
        forEachKey([](auto key)
        {
            if(auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::ScanUnknown)
                if(auto key2 = sf::Keyboard::localize(scancode); key2 != sf::Keyboard::Unknown && key2 != key)
                    std::cout << std::setw(10) << key << " -> " << std::setw(22) << scancode << " -> " << key2 << '\n';
        });
        std::cout << '\n';

        std::cout << "\tScancodes for which localize(scancode) == Unknown\n\n";
        forEachScancode([](auto scancode)
        {
            if(auto key = sf::Keyboard::localize(scancode); key == sf::Keyboard::Unknown)
                std::cout << std::setw(22) << scancode << " -> " << key << '\n';
        });
        std::cout << '\n';

        std::cout << "\tOther scancodes for which delocalize(localize(scancode)) == ScanUnknown\n\n";
        forEachScancode([](auto scancode)
        {
            if(auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
                if(auto scancode2 = sf::Keyboard::delocalize(key); scancode2 == sf::Keyboard::ScanUnknown)
                    std::cout << std::setw(22) << scancode << " -> " << std::setw(10) << key << " -> " << scancode2 << '\n';
        });
        std::cout << '\n';

        std::cout << "\tOther scancodes for which delocalize(localize(scancode)) != scancode\n\n";
        forEachScancode([](auto scancode)
        {
            if(auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
                if(auto scancode2 = sf::Keyboard::delocalize(key); scancode2 != sf::Keyboard::ScanUnknown && scancode2 != scancode)
                    std::cout << std::setw(22) << scancode << " -> " << std::setw(10) << key << " -> " << scancode2 << '\n';
        });
        std::cout << '\n';
    }

    // Generate dot diagram about localize and delocalize
    if(args.generateDiagram)
    {
        auto inDegreeScancode = std::array<int, sf::Keyboard::ScancodeCount>{};
        forEachKey([&inDegreeScancode](auto key)
        {
            if(auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::ScanUnknown)
                inDegreeScancode[scancode]++;
        });

        auto inDegreeKey = std::array<int, sf::Keyboard::KeyCount>{};
        forEachScancode([&inDegreeKey](auto scancode)
        {
            if(auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
                inDegreeKey[key]++;
        });

        auto diagram = std::ofstream("diagram.dot");
        diagram << "digraph {\n"
                << "rankdir = LR;\n"
                << "node [shape=box]\n";

        forEachKey([&diagram, &inDegreeScancode, &inDegreeKey](auto key)
        {
            if(auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::ScanUnknown)
            {
                auto key2 = sf::Keyboard::localize(scancode);
                if(inDegreeScancode[scancode] != 1 || inDegreeKey[key] != 1 || key != key2)
                    diagram << key << " -> " << scancode << '\n';
            }
            else
            {
                diagram << key << " {rank=min " << key << "}\n";
            }
        });

        forEachScancode([&diagram, &inDegreeScancode, &inDegreeKey](auto scancode)
        {
            if(auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
            {
                auto scancode2 = sf::Keyboard::delocalize(key);
                if(inDegreeKey[key] != 1 || inDegreeScancode[scancode] != 1 || scancode != scancode2)
                    diagram << key << " -> " << scancode << " [dir=back]\n";
            }
            else
            {
                // Commented to get a small graph because a lot of scancodes are mapped to Unknown.
                //diagram << scancode << " {rank=max " << scancode << "}\n";
            }
        });

        diagram << "}\n";
    }

    // Load resources, setup sf::Sound and sf::Text instances
    const auto resources_path = std::filesystem::path{ "resources" };

    sf::SoundBuffer errorSoundBuffer, pressedSoundBuffer, releasedSoundBuffer;
    if (!errorSoundBuffer.loadFromFile((resources_path / "error_005.ogg").string()) ||
        !pressedSoundBuffer.loadFromFile((resources_path / "mouseclick1.ogg").string()) ||
        !releasedSoundBuffer.loadFromFile((resources_path / "mouserelease1.ogg").string()))
    {
        std::cerr << "Failed to load sound" << std::endl;
        return 1;
    }

    auto errorSound    = sf::Sound{ errorSoundBuffer },
         pressedSound  = sf::Sound{ pressedSoundBuffer },
         releasedSound = sf::Sound{ releasedSoundBuffer };

    auto font = sf::Font{};
    if (!font.loadFromFile((resources_path / "Tuffy.ttf").string()))
    {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    auto makeText = [&font](const sf::String& string)
    {
        constexpr auto textSize = 14u, space = 4u;
        constexpr auto spacingFactor = static_cast<float>(textSize + space) / textSize;

        auto text = sf::Text{ string, font, textSize };
        text.setLineSpacing(spacingFactor);

        return text;
    };

    constexpr auto columnSize = 300u;

    auto keyPressedText = makeText("Key Pressed\n");
    keyPressedText.setPosition({ 0.f, 0.f });
    auto keyReleasedText = makeText("Key Released\n");
    keyReleasedText.setPosition({ 0.f, 200.f });

    auto keyPressedCheckText = std::array<sf::Text, 2>{};
    keyPressedCheckText.fill(makeText("IsKeyPressed sf::Keyboard::Key\n"));
    keyPressedCheckText[0].setPosition({ 1 * columnSize, 0.f });
    keyPressedCheckText[1].setPosition({ 2 * columnSize, 0.f });

    auto keyPressedScancodeCheckText = std::array<sf::Text, 3>{};
    keyPressedScancodeCheckText.fill(makeText("IsKeyPressed sf::Keyboard::Scancode\n"));
    keyPressedScancodeCheckText[0].setPosition({ 3 * columnSize, 0.f });
    keyPressedScancodeCheckText[1].setPosition({ 4 * columnSize, 0.f });
    keyPressedScancodeCheckText[2].setPosition({ 5 * columnSize, 0.f });

    auto mouseButtonPressedText = makeText("Mouse Button Pressed\n");
    mouseButtonPressedText.setPosition({ 0.f, 400.f });
    auto mouseButtonReleasedText = makeText("Mouse Button Released\n");
    mouseButtonReleasedText.setPosition({ 0.f, 500.f });
    auto mouseButtonPressedCheckText = makeText("IsButtonPressed\n");
    mouseButtonPressedCheckText.setPosition({ 0.f, 600.f });

    auto keys = std::array<bool, sf::Keyboard::KeyCount>{};
    auto scancodeKeys = std::array<bool, sf::Keyboard::ScancodeCount>{};

    // Main loop
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

        forEachKey([&keys](auto key)
        {
            keys[static_cast<unsigned>(key)] = sf::Keyboard::isKeyPressed(key);
        });

        {
            constexpr auto keyBounds = std::array
            {
                static_cast<sf::Keyboard::Key>(0),
                sf::Keyboard::Apostrophe,
                sf::Keyboard::KeyCount
            };

            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Key" };
            for (auto b = 0u; b < keyBounds.size() - 1; ++b)
            {
                text += "\n\nCode / Description / Delocalized / Pressed\n";
                forEachEnum(keyBounds[b], keyBounds[b + 1], [&keys, &text](auto key)
                {
                    auto keyPressed = keys[static_cast<unsigned>(key)];
                    text += keyDescription(key, keyPressed);
                });

                keyPressedCheckText[b].setString(text);
                text.clear();
            }
        }

        forEachScancode([&scancodeKeys](auto scancode)
        {
            scancodeKeys[static_cast<unsigned>(scancode)] = sf::Keyboard::isKeyPressed(scancode);
        });

        {
            constexpr auto scancodeBounds = std::array
            {
                static_cast<sf::Keyboard::Scancode>(0),
                sf::Keyboard::ScanComma,
                sf::Keyboard::ScanNumpad1,
                sf::Keyboard::ScancodeCount
            };

            auto text = sf::String{ "IsKeyPressed sf::Keyboard::Scancode" };
            for (auto b = 0u; b < scancodeBounds.size() - 1; ++b)
            {
                text += "\n\nScanCode / Description / Localized / Pressed\n";
                forEachEnum(scancodeBounds[b], scancodeBounds[b + 1], [&scancodeKeys, &text](auto scancode)
                {
                    auto scancodeKeyPressed = scancodeKeys[static_cast<unsigned>(scancode)];
                    text += scancodeDescription(scancode, scancodeKeyPressed);
                });

                keyPressedScancodeCheckText[b].setString(text);
                text.clear();
            }
        }

        {
            auto text = sf::String{ "IsButtonPressed sf::Mouse::Button\n\n" };
            forEachEnum(static_cast<sf::Mouse::Button>(0), sf::Mouse::ButtonCount, [&text](auto button)
            {
                auto buttonPressed = sf::Mouse::isButtonPressed(button);
                text += buttonDescription(button, buttonPressed);
            });

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

Arguments::Arguments(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        const auto arg = std::string{ argv[i] };

        if (arg == "-h" || arg == "--help")
            help = true;
        else if (arg == "-v" || arg == "--verbose")
            verbose = true;
        else if (arg == "-d" || arg == "--dot")
            generateDiagram = true;
        else if (arg == "-u" || arg == "--utf8")
            utf8 = true;
        else
        {
            help = true;
            std::cout << "Error: unknown argument " << arg << '\n';
        }
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

        // No default case on purpose so that compilers can report unhandled values.
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

std::ostream& operator<<(std::ostream& os, sf::Keyboard::Key code)
{
    return os << keyIdentifier(code);
}

std::ostream& operator<<(std::ostream& os, sf::Keyboard::Scancode scancode)
{
    return os << scancodeIdentifier(scancode);
}

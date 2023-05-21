#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

constexpr auto help =
    "  -v, --verbose   Show more information\n"
    "  -d, --dot       Generate dot diagram about localize and delocalize functions\n"
    "  -u, --utf8      Encode console output as UTF-8 instead of ANSI\n"
    "  -h, --help      Show help and exit";

struct Arguments
{
    Arguments(int argc, char* argv[]);

    bool verbose         = false;
    bool generateDiagram = false;
    bool utf8            = false;
    bool help            = false;
};

std::string encodeStringToAnsi(const sf::String& string);
std::string encodeStringToUtf8(const sf::String& string);

sf::String keyEventDescription(sf::String init, const sf::Event::KeyEvent& keyEvent);
sf::String textEventDescription(const sf::Event::TextEvent& textEvent);
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

template <typename Enum>
class EnumRange
{
public:
    class Iterator
    {
    public:
        constexpr explicit Iterator(Enum value) : m_value{value}
        {
        }

        constexpr Enum operator*() const
        {
            return static_cast<Enum>(m_value);
        }
        constexpr Iterator& operator++()
        {
            ++m_value;
            return *this;
        }
        constexpr bool operator!=(const Iterator& other) const
        {
            return *(*this) != *other;
        }

    private:
        typename std::underlying_type_t<Enum> m_value;
    };

    constexpr explicit EnumRange(Enum begin, Enum end) : m_begin{begin}, m_end{end}
    {
    }

    constexpr Iterator begin() const
    {
        return Iterator{m_begin};
    }
    constexpr Iterator end() const
    {
        return Iterator{m_end};
    }

private:
    const Enum m_begin, m_end;
};

constexpr auto keys      = EnumRange{static_cast<sf::Keyboard::Key>(0), sf::Keyboard::KeyCount};
constexpr auto scancodes = EnumRange{static_cast<sf::Keyboard::Scancode>(0), sf::Keyboard::Scan::ScancodeCount};
constexpr auto buttons   = EnumRange{static_cast<sf::Mouse::Button>(0), sf::Mouse::ButtonCount};

class ShinyText : public sf::Text
{
public:
    using sf::Text::Text;

    void shine(const sf::Color& color = sf::Color::Yellow)
    {
        setOutlineColor(color);
        m_remaining = m_duration;
    }

    void update(sf::Time frameTime)
    {
        const float ratio = m_remaining / m_duration;
        const float alpha = std::max(0.f, ratio * (2.f - ratio)) * 0.5f;

        auto color = getOutlineColor();
        color.a    = 255 * alpha;
        setOutlineColor(color);

        if (sf::Time::Zero < m_remaining)
            m_remaining -= frameTime;
    }

private:
    const sf::Time m_duration = sf::milliseconds(150);
    sf::Time       m_remaining;
};

int main(int argc, char* argv[])
{
    const auto args = Arguments{argc, argv};
    if (args.help)
    {
        std::cout << "Usage: " << argv[0] << " [OPTION]...\n\n" << help << '\n';

        return 0;
    }

    const auto encode = args.utf8 ? encodeStringToUtf8 : encodeStringToAnsi;

    auto window = sf::RenderWindow{{1920, 1200}, "SFML Input Test"};
    window.setFramerateLimit(15);

    std::cout << "\tScancode descriptions\n\n";
    for (auto scancode : scancodes)
        std::cout << std::right << std::setw(3) << static_cast<int>(scancode) << ' ' << std::left << std::setw(24)
                  << scancode << ' ' << encode(sf::Keyboard::getDescription(scancode)) << '\n';
    std::cout << '\n';

    // Output information about localize and delocalize
    if (args.verbose)
    {
        std::cout << "\tKeys for which delocalize(key) == Scan::Unknown\n\n";
        for (auto key : keys)
            if (auto scancode = sf::Keyboard::delocalize(key); scancode == sf::Keyboard::Scan::Unknown)
                std::cout << std::setw(10) << key << " -> " << scancode << '\n';
        std::cout << '\n';

        std::cout << "\tOther keys for which localize(delocalize(key)) == Unknown\n\n";
        for (auto key : keys)
            if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
                if (auto key2 = sf::Keyboard::localize(scancode); key2 == sf::Keyboard::Unknown)
                    std::cout << std::setw(10) << key << " -> " << std::setw(24) << scancode << " -> " << key2 << '\n';
        std::cout << '\n';

        std::cout << "\tOther keys for which localize(delocalize(key)) != key\n\n";
        for (auto key : keys)
            if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
                if (auto key2 = sf::Keyboard::localize(scancode); key2 != sf::Keyboard::Unknown && key2 != key)
                    std::cout << std::setw(10) << key << " -> " << std::setw(24) << scancode << " -> " << key2 << '\n';
        std::cout << '\n';

        std::cout << "\tScancodes for which localize(scancode) == Unknown\n\n";
        for (auto scancode : scancodes)
            if (auto key = sf::Keyboard::localize(scancode); key == sf::Keyboard::Unknown)
                std::cout << std::setw(24) << scancode << " -> " << key << '\n';
        std::cout << '\n';

        std::cout << "\tOther scancodes for which delocalize(localize(scancode)) == Scan::Unknown\n\n";
        for (auto scancode : scancodes)
            if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
                if (auto scancode2 = sf::Keyboard::delocalize(key); scancode2 == sf::Keyboard::Scan::Unknown)
                    std::cout << std::setw(24) << scancode << " -> " << std::setw(10) << key << " -> " << scancode2 << '\n';
        std::cout << '\n';

        std::cout << "\tOther scancodes for which delocalize(localize(scancode)) != scancode\n\n";
        for (auto scancode : scancodes)
            if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
                if (auto scancode2 = sf::Keyboard::delocalize(key);
                    scancode2 != sf::Keyboard::Scan::Unknown && scancode2 != scancode)
                    std::cout << std::setw(24) << scancode << " -> " << std::setw(10) << key << " -> " << scancode2 << '\n';
        std::cout << '\n';
    }

    // Generate dot diagram about localize and delocalize
    if (args.generateDiagram)
    {
        auto inDegreeScancode = std::array<int, sf::Keyboard::Scan::ScancodeCount>{};
        for (auto key : keys)
            if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
                inDegreeScancode[scancode]++;

        auto inDegreeKey = std::array<int, sf::Keyboard::KeyCount>{};
        for (auto scancode : scancodes)
            if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
                inDegreeKey[key]++;

        auto diagram = std::ofstream("diagram.dot");
        diagram << "digraph {\n"
                << "rankdir=LR\n"
                << "node [shape=box fontname=monospace]\n";

        for (auto key : keys)
            if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
            {
                auto key2 = sf::Keyboard::localize(scancode);
                if (inDegreeScancode[scancode] != 1 || inDegreeKey[key] != 1 || key != key2)
                    diagram << key << " -> \"" << scancode << "\"\n";
            }
            else
            {
                diagram << key << " {rank=min " << key << "}\n";
            }

        for (auto scancode : scancodes)
            if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Unknown)
            {
                auto scancode2 = sf::Keyboard::delocalize(key);
                if (inDegreeKey[key] != 1 || inDegreeScancode[scancode] != 1 || scancode != scancode2)
                    diagram << key << " -> \"" << scancode << "\" [dir=back]\n";
            }
            else
            {
                // Commented to get a small graph because a lot of scancodes are mapped to Unknown.
                //diagram << '"' << scancode << "\" {rank=max \"" << scancode << "\"}\n";
            }

        diagram << "}\n";
    }

    // Load resources, setup sf::Sound and sf::Text instances
    const auto resources_path = std::filesystem::path{"resources"};

    sf::SoundBuffer errorSoundBuffer, pressedSoundBuffer, releasedSoundBuffer;
    if (!errorSoundBuffer.loadFromFile((resources_path / "error_005.ogg").string()) ||
        !pressedSoundBuffer.loadFromFile((resources_path / "mouseclick1.ogg").string()) ||
        !releasedSoundBuffer.loadFromFile((resources_path / "mouserelease1.ogg").string()))
    {
        std::cerr << "Failed to load sound" << std::endl;
        return 1;
    }

    auto errorSound = sf::Sound{errorSoundBuffer}, pressedSound = sf::Sound{pressedSoundBuffer},
         releasedSound = sf::Sound{releasedSoundBuffer};

    auto font = sf::Font{};
    if (!font.loadFromFile((resources_path / "Tuffy.ttf").string()))
    {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    constexpr auto textSize = 14u, space = 4u;
    constexpr auto lineSize      = textSize + space;
    const auto     spacingFactor = static_cast<float>(lineSize) / font.getLineSpacing(textSize);

    auto makeText = [&](const sf::String& string)
    {
        auto text = sf::Text{string, font, textSize};
        text.setLineSpacing(spacingFactor);

        return text;
    };

    auto makeShinyText = [&](const sf::String& string)
    {
        auto text = ShinyText{string, font, textSize};
        text.setLineSpacing(spacingFactor);
        text.setOutlineThickness(2.f);

        return text;
    };

    constexpr auto columnSize = 300u;

    auto keyPressedText = makeShinyText("Key Pressed\n");
    keyPressedText.setPosition({0.f, 0.f});
    auto textEnteredText = makeShinyText("Text Entered\n");
    textEnteredText.setPosition({0.f, 8 * lineSize});
    auto keyReleasedText = makeShinyText("Key Released\n");
    keyReleasedText.setPosition({0.f, 12 * lineSize});

    auto keyPressedCheckText = std::array<sf::Text, 2>{};
    keyPressedCheckText.fill(makeText("IsKeyPressed sf::Keyboard::Key\n"));
    keyPressedCheckText[0].setPosition({1 * columnSize, 0.f});
    keyPressedCheckText[1].setPosition({2 * columnSize, 0.f});

    auto keyPressedScancodeCheckText = std::array<sf::Text, 3>{};
    keyPressedScancodeCheckText.fill(makeText("IsKeyPressed sf::Keyboard::Scancode\n"));
    keyPressedScancodeCheckText[0].setPosition({3 * columnSize, 0.f});
    keyPressedScancodeCheckText[1].setPosition({4 * columnSize, 0.f});
    keyPressedScancodeCheckText[2].setPosition({5 * columnSize, 0.f});

    auto mouseButtonPressedText = makeShinyText("Mouse Button Pressed\n");
    mouseButtonPressedText.setPosition({0.f, 400.f});
    auto mouseButtonReleasedText = makeShinyText("Mouse Button Released\n");
    mouseButtonReleasedText.setPosition({0.f, 500.f});
    auto mouseButtonPressedCheckText = makeText("IsButtonPressed\n");
    mouseButtonPressedCheckText.setPosition({0.f, 600.f});

    auto keyPressed      = std::array<bool, sf::Keyboard::KeyCount>{};
    auto scancodePressed = std::array<bool, sf::Keyboard::Scan::ScancodeCount>{};

    auto lastEventKey      = sf::Keyboard::Unknown;
    auto lastEventScancode = sf::Keyboard::Scan::Unknown;

    // Main loop
    sf::Clock clock;
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                const auto size = sf::Vector2u{event.size.width, event.size.height};
                window.setView(sf::View(sf::FloatRect({}, sf::Vector2f{size})));
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                auto text = keyEventDescription("Key Pressed", event.key);

                keyPressedText.setString(text);
                std::cout << encode(text);

                lastEventKey      = event.key.code;
                lastEventScancode = event.key.scancode;

                if (seemsStrange(event.key))
                {
                    keyPressedText.shine(sf::Color::Red);
                    errorSound.play();
                }
                else
                {
                    keyPressedText.shine(sf::Color::Green);
                    pressedSound.play();
                }
            }
            else if (event.type == sf::Event::TextEntered)
            {
                auto text = textEventDescription(event.text);

                textEnteredText.setString(text);
                std::cout << encode(text);

                textEnteredText.shine();
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                auto text = keyEventDescription("Key Released", event.key);

                keyReleasedText.setString(text);
                std::cout << encode(text);

                lastEventKey      = event.key.code;
                lastEventScancode = event.key.scancode;

                if (seemsStrange(event.key))
                {
                    keyReleasedText.shine(sf::Color::Red);
                    errorSound.play();
                }
                else
                {
                    keyReleasedText.shine(sf::Color::Green);
                    releasedSound.play();
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                auto text = buttonEventDescription("Mouse Button Pressed", event.mouseButton);

                mouseButtonPressedText.setString(text);
                std::cout << encode(text);

                mouseButtonPressedText.shine();
                pressedSound.play();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                auto text = buttonEventDescription("Mouse Button Released", event.mouseButton);

                mouseButtonReleasedText.setString(text);
                std::cout << encode(text);

                mouseButtonReleasedText.shine();
                releasedSound.play();
            }
        }

        const sf::Time frameTime = clock.restart();

        keyPressedText.update(frameTime);
        textEnteredText.update(frameTime);
        keyReleasedText.update(frameTime);
        mouseButtonPressedText.update(frameTime);
        mouseButtonReleasedText.update(frameTime);

        for (auto key : keys)
            keyPressed[static_cast<unsigned>(key)] = sf::Keyboard::isKeyPressed(key);

        {
            constexpr auto keyBounds = std::array{static_cast<sf::Keyboard::Key>(0),
                                                  sf::Keyboard::Apostrophe,
                                                  sf::Keyboard::KeyCount};

            auto text = sf::String{"IsKeyPressed sf::Keyboard::Key"};
            for (auto b = 0u; b < keyBounds.size() - 1; ++b)
            {
                text += "\n\nCode / Description / Delocalized / Pressed\n";
                for (auto key : EnumRange{keyBounds[b], keyBounds[b + 1]})
                {
                    if (key == lastEventKey)
                        text += "*\t";
                    text += keyDescription(key, keyPressed[static_cast<unsigned>(key)]);
                }

                keyPressedCheckText[b].setString(text);
                text.clear();
            }
        }

        for (auto scancode : scancodes)
            scancodePressed[static_cast<unsigned>(scancode)] = sf::Keyboard::isKeyPressed(scancode);

        {
            constexpr auto scancodeBounds = std::array{static_cast<sf::Keyboard::Scancode>(0),
                                                       sf::Keyboard::Scan::Comma,
                                                       sf::Keyboard::Scan::Numpad1,
                                                       sf::Keyboard::Scan::ScancodeCount};

            auto text = sf::String{"IsKeyPressed sf::Keyboard::Scancode"};
            for (auto b = 0u; b < scancodeBounds.size() - 1; ++b)
            {
                text += "\n\nScanCode / Description / Localized / Pressed\n";
                for (auto scancode : EnumRange{scancodeBounds[b], scancodeBounds[b + 1]})
                {
                    if (scancode == lastEventScancode)
                        text += "*\t";
                    text += scancodeDescription(scancode, scancodePressed[static_cast<unsigned>(scancode)]);
                }

                keyPressedScancodeCheckText[b].setString(text);
                text.clear();
            }
        }

        {
            auto text = sf::String{"IsButtonPressed sf::Mouse::Button\n\n"};
            for (auto button : buttons)
                text += buttonDescription(button, sf::Mouse::isButtonPressed(button));

            mouseButtonPressedCheckText.setString(text);
        }

        window.clear();

        window.draw(keyPressedText);
        window.draw(textEnteredText);
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
        const auto arg = std::string{argv[i]};

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

sf::String textEventDescription(const sf::Event::TextEvent& textEvent)
{
    sf::String text = "Text Entered";
    text += "\n\nunicode:\t";
    text += std::to_string(textEvent.unicode);
    text += "\t";
    text += textEvent.unicode;
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
    return keyEvent.code == -1 || keyEvent.scancode == -1 || sf::Keyboard::getDescription(keyEvent.scancode) == "" ||
           sf::Keyboard::localize(keyEvent.scancode) != keyEvent.code ||
           sf::Keyboard::delocalize(keyEvent.code) != keyEvent.scancode;
}

std::string keyIdentifier(sf::Keyboard::Key code)
{
    // Inspired by mantognini/SFML-Test-Events

    static_assert(sf::Keyboard::KeyCount == 101,
                  "Number of SFML keys has changed. The switch statement must be updated.");

    switch (code)
    {
#define CASE(code)           \
    case sf::Keyboard::code: \
        return #code
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
        CASE(Grave);
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
            throw std::runtime_error{"invalid keyboard code"};

            // No default case on purpose so that compilers can report unhandled values.
    }

    throw std::runtime_error{"invalid keyboard code"};
}

std::string scancodeIdentifier(sf::Keyboard::Scancode scancode)
{
    // Same design as the keyIdentifier function

    static_assert(sf::Keyboard::Scan::ScancodeCount == 146,
                  "Number of SFML scancodes has changed. The switch statement must be updated.");

    switch (scancode)
    {
#define CASE(scancode)           \
    case sf::Keyboard::scancode: \
        return #scancode
        CASE(Scan::Unknown);
        CASE(Scan::A);
        CASE(Scan::B);
        CASE(Scan::C);
        CASE(Scan::D);
        CASE(Scan::E);
        CASE(Scan::F);
        CASE(Scan::G);
        CASE(Scan::H);
        CASE(Scan::I);
        CASE(Scan::J);
        CASE(Scan::K);
        CASE(Scan::L);
        CASE(Scan::M);
        CASE(Scan::N);
        CASE(Scan::O);
        CASE(Scan::P);
        CASE(Scan::Q);
        CASE(Scan::R);
        CASE(Scan::S);
        CASE(Scan::T);
        CASE(Scan::U);
        CASE(Scan::V);
        CASE(Scan::W);
        CASE(Scan::X);
        CASE(Scan::Y);
        CASE(Scan::Z);
        CASE(Scan::Num1);
        CASE(Scan::Num2);
        CASE(Scan::Num3);
        CASE(Scan::Num4);
        CASE(Scan::Num5);
        CASE(Scan::Num6);
        CASE(Scan::Num7);
        CASE(Scan::Num8);
        CASE(Scan::Num9);
        CASE(Scan::Num0);
        CASE(Scan::Enter);
        CASE(Scan::Escape);
        CASE(Scan::Backspace);
        CASE(Scan::Tab);
        CASE(Scan::Space);
        CASE(Scan::Hyphen);
        CASE(Scan::Equal);
        CASE(Scan::LBracket);
        CASE(Scan::RBracket);
        CASE(Scan::Backslash);
        CASE(Scan::Semicolon);
        CASE(Scan::Apostrophe);
        CASE(Scan::Grave);
        CASE(Scan::Comma);
        CASE(Scan::Period);
        CASE(Scan::Slash);
        CASE(Scan::F1);
        CASE(Scan::F2);
        CASE(Scan::F3);
        CASE(Scan::F4);
        CASE(Scan::F5);
        CASE(Scan::F6);
        CASE(Scan::F7);
        CASE(Scan::F8);
        CASE(Scan::F9);
        CASE(Scan::F10);
        CASE(Scan::F11);
        CASE(Scan::F12);
        CASE(Scan::F13);
        CASE(Scan::F14);
        CASE(Scan::F15);
        CASE(Scan::F16);
        CASE(Scan::F17);
        CASE(Scan::F18);
        CASE(Scan::F19);
        CASE(Scan::F20);
        CASE(Scan::F21);
        CASE(Scan::F22);
        CASE(Scan::F23);
        CASE(Scan::F24);
        CASE(Scan::CapsLock);
        CASE(Scan::PrintScreen);
        CASE(Scan::ScrollLock);
        CASE(Scan::Pause);
        CASE(Scan::Insert);
        CASE(Scan::Home);
        CASE(Scan::PageUp);
        CASE(Scan::Delete);
        CASE(Scan::End);
        CASE(Scan::PageDown);
        CASE(Scan::Right);
        CASE(Scan::Left);
        CASE(Scan::Down);
        CASE(Scan::Up);
        CASE(Scan::NumLock);
        CASE(Scan::NumpadDivide);
        CASE(Scan::NumpadMultiply);
        CASE(Scan::NumpadMinus);
        CASE(Scan::NumpadPlus);
        CASE(Scan::NumpadEqual);
        CASE(Scan::NumpadEnter);
        CASE(Scan::NumpadDecimal);
        CASE(Scan::Numpad1);
        CASE(Scan::Numpad2);
        CASE(Scan::Numpad3);
        CASE(Scan::Numpad4);
        CASE(Scan::Numpad5);
        CASE(Scan::Numpad6);
        CASE(Scan::Numpad7);
        CASE(Scan::Numpad8);
        CASE(Scan::Numpad9);
        CASE(Scan::Numpad0);
        CASE(Scan::NonUsBackslash);
        CASE(Scan::Application);
        CASE(Scan::Execute);
        CASE(Scan::ModeChange);
        CASE(Scan::Help);
        CASE(Scan::Menu);
        CASE(Scan::Select);
        CASE(Scan::Redo);
        CASE(Scan::Undo);
        CASE(Scan::Cut);
        CASE(Scan::Copy);
        CASE(Scan::Paste);
        CASE(Scan::VolumeMute);
        CASE(Scan::VolumeUp);
        CASE(Scan::VolumeDown);
        CASE(Scan::MediaPlayPause);
        CASE(Scan::MediaStop);
        CASE(Scan::MediaNextTrack);
        CASE(Scan::MediaPreviousTrack);
        CASE(Scan::LControl);
        CASE(Scan::LShift);
        CASE(Scan::LAlt);
        CASE(Scan::LSystem);
        CASE(Scan::RControl);
        CASE(Scan::RShift);
        CASE(Scan::RAlt);
        CASE(Scan::RSystem);
        CASE(Scan::Back);
        CASE(Scan::Forward);
        CASE(Scan::Refresh);
        CASE(Scan::Stop);
        CASE(Scan::Search);
        CASE(Scan::Favorites);
        CASE(Scan::HomePage);
        CASE(Scan::LaunchApplication1);
        CASE(Scan::LaunchApplication2);
        CASE(Scan::LaunchMail);
        CASE(Scan::LaunchMediaSelect);
#undef CASE

        case sf::Keyboard::Scan::ScancodeCount:
            throw std::runtime_error{"invalid keyboard scancode"};
    }

    throw std::runtime_error{"invalid keyboard scancode"};
}

std::string buttonIdentifier(sf::Mouse::Button button)
{
    // Same design as the keyIdentifier function

    static_assert(sf::Mouse::ButtonCount == 5,
                  "Number of SFML mouse buttons has changed. The switch statement must be updated.");

    switch (button)
    {
#define CASE(button)        \
    case sf::Mouse::button: \
        return #button
        CASE(Left);
        CASE(Right);
        CASE(Middle);
        CASE(XButton1);
        CASE(XButton2);
#undef CASE

        case sf::Mouse::ButtonCount:
            throw std::runtime_error{"invalid mouse button"};
    }

    throw std::runtime_error{"invalid mouse button"};
}

std::ostream& operator<<(std::ostream& os, sf::Keyboard::Key code)
{
    return os << keyIdentifier(code);
}

std::ostream& operator<<(std::ostream& os, sf::Keyboard::Scancode scancode)
{
    return os << scancodeIdentifier(scancode);
}

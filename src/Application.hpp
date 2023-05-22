#pragma once

#include "ShinyText.hpp"
#include "strings.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <SFML/Window/Event.hpp>

#include <array>
#include <filesystem>
#include <optional>

struct Resources
{
    static std::optional<Resources> load(const std::filesystem::path& resourcesPath);

    sf::SoundBuffer errorSoundBuffer;
    sf::SoundBuffer pressedSoundBuffer;
    sf::SoundBuffer releasedSoundBuffer;
    sf::Font        font;
};

class Application
{
public:
    Application(const Resources& resources, Encoder encode);

    int run();

private:
    void handle(const sf::Event& event);
    void update(sf::Time frameTime);
    void render();

private:
    sf::RenderWindow window{{1920, 1200}, "SFML Input Test"};
    const Resources& resources;
    const Encoder    encode;

    sf::Sound errorSound{resources.errorSoundBuffer};
    sf::Sound pressedSound{resources.pressedSoundBuffer};
    sf::Sound releasedSound{resources.releasedSoundBuffer};

    ShinyText keyPressedText, textEnteredText, keyReleasedText;
    ShinyText mouseButtonPressedText, mouseButtonReleasedText;

    std::array<sf::Text, 2> keyPressedCheckText;
    std::array<sf::Text, 3> keyPressedScancodeCheckText;
    sf::Text                mouseButtonPressedCheckText;

    sf::Keyboard::Key      lastEventKey{sf::Keyboard::Unknown};
    sf::Keyboard::Scancode lastEventScancode{sf::Keyboard::Scan::Unknown};
};

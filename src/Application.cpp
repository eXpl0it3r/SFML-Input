#include "Application.hpp"

#include "ranges.hpp"
#include "strings.hpp"

#include <SFML/Window/VideoMode.hpp>

#include <iostream>

namespace
{
sf::String keyEventDescription(sf::String text, const sf::Event::KeyEvent& keyEvent)
{
    text += "\n\nCode:\t\t";
    text += std::to_string(static_cast<int>(keyEvent.code));
    text += "\tsf::Keyboard::";
    text += keyIdentifier(keyEvent.code);
    text += "\nScanCode:\t";
    text += std::to_string(static_cast<int>(keyEvent.scancode));
    text += "\tsf::Keyboard::";
    text += scancodeIdentifier(keyEvent.scancode);
    text += "\nDescription:\t";
    text += sf::Keyboard::getDescription(keyEvent.scancode);
    text += "\nLocalized:\t";
    text += std::to_string(static_cast<int>(sf::Keyboard::localize(keyEvent.scancode)));
    text += "\tsf::Keyboard::";
    text += keyIdentifier(sf::Keyboard::localize(keyEvent.scancode));
    text += "\nDelocalized:\t";
    text += std::to_string(static_cast<int>(sf::Keyboard::delocalize(keyEvent.code)));
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
    text += static_cast<char32_t>(textEvent.unicode);
    text += "\n\n";

    return text;
}

sf::String buttonEventDescription(sf::String text, const sf::Event::MouseButtonEvent& buttonEvent)
{
    text += "\n\nButton:\t";
    text += std::to_string(static_cast<int>(buttonEvent.button));
    text += "\tsf::Mouse::";
    text += buttonIdentifier(buttonEvent.button);
    text += "\n\n";

    return text;
}

sf::String buttonDescription(sf::Mouse::Button button, bool buttonPressed)
{
    sf::String text = std::to_string(static_cast<int>(button)) + " / ";
    text += "sf::Mouse::";
    text += buttonIdentifier(button);
    text += buttonPressed ? "\tPressed" : "";
    text += "\n";

    return text;
}

bool seemsStrange(const sf::Event::KeyEvent& keyEvent)
{
    return keyEvent.code == sf::Keyboard::Key::Unknown || keyEvent.scancode == sf::Keyboard::Scan::Unknown ||
           sf::Keyboard::getDescription(keyEvent.scancode) == "" ||
           sf::Keyboard::localize(keyEvent.scancode) != keyEvent.code ||
           sf::Keyboard::delocalize(keyEvent.code) != keyEvent.scancode;
}

static constexpr auto textSize{14u};
static constexpr auto space{4u};
static constexpr auto lineSize{textSize + space};

float getSpacingFactor(const sf::Font& font)
{
    return static_cast<float>(lineSize) / font.getLineSpacing(textSize);
}

ShinyText makeShinyText(const sf::Font& font, const sf::String& string, const sf::Vector2f& position)
{
    auto text = ShinyText{font, string, textSize};
    text.setLineSpacing(getSpacingFactor(font));
    text.setOutlineThickness(2.f);
    text.setPosition(position);

    return text;
}

sf::Text makeText(const sf::Font& font, const sf::String& string, const sf::Vector2f& position)
{
    auto text = sf::Text{font, string, textSize};
    text.setLineSpacing(getSpacingFactor(font));
    text.setPosition(position);

    return text;
}

} // namespace

std::optional<Resources> Resources::load(const std::filesystem::path& resourcesPath)
{
    auto resources = std::optional<Resources>{std::in_place};

    if (!resources->errorSoundBuffer.loadFromFile((resourcesPath / "error_005.ogg").string()) ||
        !resources->pressedSoundBuffer.loadFromFile((resourcesPath / "mouseclick1.ogg").string()) ||
        !resources->releasedSoundBuffer.loadFromFile((resourcesPath / "mouserelease1.ogg").string()) ||
        !resources->font.loadFromFile((resourcesPath / "Tuffy.ttf").string()))
    {
        return std::nullopt;
    }

    return resources;
}

Application::Application(const Resources& resources, Encoder encode) :
window{sf::VideoMode{{1920, 1200}}, "SFML Input Test"},
resources{resources},
encode{encode},
keyPressedText{makeShinyText(resources.font, "Key Pressed", {0, 0})},
textEnteredText{makeShinyText(resources.font, "Text Entered", {0, 8 * lineSize})},
keyReleasedText{makeShinyText(resources.font, "Key Released", {0, 12 * lineSize})},
keyPressedCheckText{makeText(resources.font, "", {0, 20 * lineSize})},
mouseButtonPressedText{makeShinyText(resources.font, "Mouse Button Pressed", {0, 30 * lineSize})},
mouseButtonReleasedText{makeShinyText(resources.font, "Mouse Button Released", {0, 34 * lineSize})},
mouseButtonPressedCheckText{makeText(resources.font, "", {0, 38 * lineSize})}
{
    window.setFramerateLimit(15);
    keyboardView.setPosition({320, 64});
}

int Application::run()
{
    auto clock = sf::Clock{};
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
            handle(event);
        update(clock.restart());
        render();
    }

    return 0;
}

void Application::handle(const sf::Event& event)
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

    keyboardView.handle(event);
}

void Application::update(sf::Time frameTime)
{
    keyPressedText.update(frameTime);
    textEnteredText.update(frameTime);
    keyReleasedText.update(frameTime);
    mouseButtonPressedText.update(frameTime);
    mouseButtonReleasedText.update(frameTime);

    {
        auto text = sf::String{"isKeyPressed(sf::Keyboard::Key)\n\n"};
        for (auto key : keys)
        {
            if (sf::Keyboard::isKeyPressed(key))
                text += "sf::Keyboard::" + keyIdentifier(key) + "\n";
        }
        keyPressedCheckText.setString(text);
    }

    {
        auto text = sf::String{"isButtonPressed(sf::Mouse::Button)\n\n"};
        for (auto button : buttons)
            text += buttonDescription(button, sf::Mouse::isButtonPressed(button));

        mouseButtonPressedCheckText.setString(text);
    }

    keyboardView.update(frameTime);
}

void Application::render()
{
    window.clear();

    window.draw(keyPressedText);
    window.draw(textEnteredText);
    window.draw(keyReleasedText);
    window.draw(keyPressedCheckText);

    window.draw(mouseButtonPressedText);
    window.draw(mouseButtonReleasedText);
    window.draw(mouseButtonPressedCheckText);

    window.draw(keyboardView);

    window.display();
}

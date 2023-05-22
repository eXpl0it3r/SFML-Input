#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <SFML/System/String.hpp>

#include <string>

using Encoder = std::string (*)(const sf::String&);
std::string encodeStringToAnsi(const sf::String& string);
std::string encodeStringToUtf8(const sf::String& string);

std::string keyIdentifier(sf::Keyboard::Key code);
std::string scancodeIdentifier(sf::Keyboard::Scancode scancode);
std::string buttonIdentifier(sf::Mouse::Button button);

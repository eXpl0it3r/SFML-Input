#include "strings.hpp"

#include <stdexcept>

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

std::string keyIdentifier(sf::Keyboard::Key code)
{
    // Inspired by mantognini/SFML-Test-Events

    static_assert(sf::Keyboard::KeyCount == 101,
                  "Number of SFML keys has changed. The switch statement must be updated.");

    switch (code)
    {
#define CASE(keycode)           \
    case sf::Keyboard::keycode: \
        return #keycode
        CASE(Key::Unknown);
        CASE(Key::A);
        CASE(Key::B);
        CASE(Key::C);
        CASE(Key::D);
        CASE(Key::E);
        CASE(Key::F);
        CASE(Key::G);
        CASE(Key::H);
        CASE(Key::I);
        CASE(Key::J);
        CASE(Key::K);
        CASE(Key::L);
        CASE(Key::M);
        CASE(Key::N);
        CASE(Key::O);
        CASE(Key::P);
        CASE(Key::Q);
        CASE(Key::R);
        CASE(Key::S);
        CASE(Key::T);
        CASE(Key::U);
        CASE(Key::V);
        CASE(Key::W);
        CASE(Key::X);
        CASE(Key::Y);
        CASE(Key::Z);
        CASE(Key::Num0);
        CASE(Key::Num1);
        CASE(Key::Num2);
        CASE(Key::Num3);
        CASE(Key::Num4);
        CASE(Key::Num5);
        CASE(Key::Num6);
        CASE(Key::Num7);
        CASE(Key::Num8);
        CASE(Key::Num9);
        CASE(Key::Escape);
        CASE(Key::LControl);
        CASE(Key::LShift);
        CASE(Key::LAlt);
        CASE(Key::LSystem);
        CASE(Key::RControl);
        CASE(Key::RShift);
        CASE(Key::RAlt);
        CASE(Key::RSystem);
        CASE(Key::Menu);
        CASE(Key::LBracket);
        CASE(Key::RBracket);
        CASE(Key::Semicolon);
        CASE(Key::Comma);
        CASE(Key::Period);
        CASE(Key::Apostrophe);
        CASE(Key::Slash);
        CASE(Key::Backslash);
        CASE(Key::Grave);
        CASE(Key::Equal);
        CASE(Key::Hyphen);
        CASE(Key::Space);
        CASE(Key::Enter);
        CASE(Key::Backspace);
        CASE(Key::Tab);
        CASE(Key::PageUp);
        CASE(Key::PageDown);
        CASE(Key::End);
        CASE(Key::Home);
        CASE(Key::Insert);
        CASE(Key::Delete);
        CASE(Key::Add);
        CASE(Key::Subtract);
        CASE(Key::Multiply);
        CASE(Key::Divide);
        CASE(Key::Left);
        CASE(Key::Right);
        CASE(Key::Up);
        CASE(Key::Down);
        CASE(Key::Numpad0);
        CASE(Key::Numpad1);
        CASE(Key::Numpad2);
        CASE(Key::Numpad3);
        CASE(Key::Numpad4);
        CASE(Key::Numpad5);
        CASE(Key::Numpad6);
        CASE(Key::Numpad7);
        CASE(Key::Numpad8);
        CASE(Key::Numpad9);
        CASE(Key::F1);
        CASE(Key::F2);
        CASE(Key::F3);
        CASE(Key::F4);
        CASE(Key::F5);
        CASE(Key::F6);
        CASE(Key::F7);
        CASE(Key::F8);
        CASE(Key::F9);
        CASE(Key::F10);
        CASE(Key::F11);
        CASE(Key::F12);
        CASE(Key::F13);
        CASE(Key::F14);
        CASE(Key::F15);
        CASE(Key::Pause);
#undef CASE
        // No default case on purpose so that compilers can report unhandled values.
    }

    throw std::runtime_error{"invalid keyboard code"};
}

std::string scancodeIdentifier(sf::Keyboard::Scancode scancode)
{
    // Same design as the keyIdentifier function

    static_assert(sf::Keyboard::ScancodeCount == 146,
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
#define CASE(button)                \
    case sf::Mouse::Button::button: \
        return #button
        CASE(Left);
        CASE(Right);
        CASE(Middle);
        CASE(Extra1);
        CASE(Extra2);
#undef CASE
    }

    throw std::runtime_error{"invalid mouse button"};
}

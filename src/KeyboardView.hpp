#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <array>
#include <vector>

class KeyboardView : public sf::Drawable, public sf::Transformable
{
public:
    KeyboardView(const sf::Font& font);
    void handle(const sf::Event& event);
    void update(sf::Time frameTime);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    static constexpr auto keySize = 64.f;
    static constexpr auto padding = 4.f;

    struct Cell
    {
        Cell(sf::Keyboard::Scancode scancode, const sf::Vector2f& size = {1.f, 1.f}, float marginRight = 0.f) :
        scancode{scancode},
        size{size * keySize},
        marginRight{marginRight * keySize}
        {
        }

        Cell(sf::Keyboard::Scancode scancode, float marginRight) : Cell{scancode, {1.f, 1.f}, marginRight}
        {
        }

        sf::Keyboard::Scancode scancode;
        sf::Vector2f           size;
        float                  marginRight;
    };

    struct Row
    {
        Row(std::vector<Cell> cells, float marginBottom = 0.f) :
        cells{std::move(cells)},
        marginBottom{marginBottom * keySize}
        {
        }

        std::vector<Cell> cells;
        float             marginBottom;
    };

    const std::array<Row, 9> matrix{{
        {{{sf::Keyboard::Scan::Escape, 1},
          {sf::Keyboard::Scan::F1},
          {sf::Keyboard::Scan::F2},
          {sf::Keyboard::Scan::F3},
          {sf::Keyboard::Scan::F4, 0.5},
          {sf::Keyboard::Scan::F5},
          {sf::Keyboard::Scan::F6},
          {sf::Keyboard::Scan::F7},
          {sf::Keyboard::Scan::F8, 0.5},
          {sf::Keyboard::Scan::F9},
          {sf::Keyboard::Scan::F10},
          {sf::Keyboard::Scan::F11},
          {sf::Keyboard::Scan::F12, 0.5},
          {sf::Keyboard::Scan::PrintScreen},
          {sf::Keyboard::Scan::ScrollLock},
          {sf::Keyboard::Scan::Pause}},
         0.5},
        {{{sf::Keyboard::Scan::Grave}, //
          {sf::Keyboard::Scan::Num1},
          {sf::Keyboard::Scan::Num2},
          {sf::Keyboard::Scan::Num3},
          {sf::Keyboard::Scan::Num4},
          {sf::Keyboard::Scan::Num5},
          {sf::Keyboard::Scan::Num6},
          {sf::Keyboard::Scan::Num7},
          {sf::Keyboard::Scan::Num8},
          {sf::Keyboard::Scan::Num9},
          {sf::Keyboard::Scan::Num0},
          {sf::Keyboard::Scan::Hyphen},
          {sf::Keyboard::Scan::Equal},
          {sf::Keyboard::Scan::Backspace, {2, 1}, 0.5},
          {sf::Keyboard::Scan::Insert},
          {sf::Keyboard::Scan::Home},
          {sf::Keyboard::Scan::PageUp, 0.5},
          {sf::Keyboard::Scan::NumLock},
          {sf::Keyboard::Scan::NumpadDivide},
          {sf::Keyboard::Scan::NumpadMultiply},
          {sf::Keyboard::Scan::NumpadMinus}}},
        {{{sf::Keyboard::Scan::Tab, {1.5, 1}},
          {sf::Keyboard::Scan::Q},
          {sf::Keyboard::Scan::W},
          {sf::Keyboard::Scan::E},
          {sf::Keyboard::Scan::R},
          {sf::Keyboard::Scan::T},
          {sf::Keyboard::Scan::Y},
          {sf::Keyboard::Scan::U},
          {sf::Keyboard::Scan::I},
          {sf::Keyboard::Scan::O},
          {sf::Keyboard::Scan::P},
          {sf::Keyboard::Scan::LBracket},
          {sf::Keyboard::Scan::RBracket},
          {sf::Keyboard::Scan::Backslash, {1.5, 1}, 0.5},
          {sf::Keyboard::Scan::Delete},
          {sf::Keyboard::Scan::End},
          {sf::Keyboard::Scan::PageDown, 0.5},
          {sf::Keyboard::Scan::Numpad7},
          {sf::Keyboard::Scan::Numpad8},
          {sf::Keyboard::Scan::Numpad9},
          {sf::Keyboard::Scan::NumpadPlus}}},
        {{{sf::Keyboard::Scan::CapsLock, {1.75, 1}},
          {sf::Keyboard::Scan::A},
          {sf::Keyboard::Scan::S},
          {sf::Keyboard::Scan::D},
          {sf::Keyboard::Scan::F},
          {sf::Keyboard::Scan::G},
          {sf::Keyboard::Scan::H},
          {sf::Keyboard::Scan::J},
          {sf::Keyboard::Scan::K},
          {sf::Keyboard::Scan::L},
          {sf::Keyboard::Scan::Semicolon},
          {sf::Keyboard::Scan::Apostrophe},
          {sf::Keyboard::Scan::Enter, {2.25, 1}, 4},
          {sf::Keyboard::Scan::Numpad4},
          {sf::Keyboard::Scan::Numpad5},
          {sf::Keyboard::Scan::Numpad6},
          {sf::Keyboard::Scan::NumpadEqual}}},
        {{{sf::Keyboard::Scan::LShift, {1.25, 1}},
          {sf::Keyboard::Scan::NonUsBackslash},
          {sf::Keyboard::Scan::Z},
          {sf::Keyboard::Scan::X},
          {sf::Keyboard::Scan::C},
          {sf::Keyboard::Scan::V},
          {sf::Keyboard::Scan::B},
          {sf::Keyboard::Scan::N},
          {sf::Keyboard::Scan::M},
          {sf::Keyboard::Scan::Comma},
          {sf::Keyboard::Scan::Period},
          {sf::Keyboard::Scan::Slash},
          {sf::Keyboard::Scan::RShift, {2.75, 1}, 1.5},
          {sf::Keyboard::Scan::Up, 1.5},
          {sf::Keyboard::Scan::Numpad1},
          {sf::Keyboard::Scan::Numpad2},
          {sf::Keyboard::Scan::Numpad3},
          {sf::Keyboard::Scan::NumpadEnter, {1, 2}}}},
        {{{sf::Keyboard::Scan::LControl, {1.5, 1}},
          {sf::Keyboard::Scan::LSystem, {1.25, 1}},
          {sf::Keyboard::Scan::LAlt, {1.5, 1}},
          {sf::Keyboard::Scan::Space, {5.75, 1}},
          {sf::Keyboard::Scan::RAlt, {1.25, 1}},
          {sf::Keyboard::Scan::RSystem, {1.25, 1}},
          {sf::Keyboard::Scan::Menu, {1.25, 1}},
          {sf::Keyboard::Scan::RControl, {1.25, 1}, 0.5},
          {sf::Keyboard::Scan::Left},
          {sf::Keyboard::Scan::Down},
          {sf::Keyboard::Scan::Right, 0.5},
          {sf::Keyboard::Scan::Numpad0, {2, 1}},
          {sf::Keyboard::Scan::NumpadDecimal}},
         1.0},
        {{{sf::Keyboard::Scan::F13},
          {sf::Keyboard::Scan::F14},
          {sf::Keyboard::Scan::F15},
          {sf::Keyboard::Scan::F16},
          {sf::Keyboard::Scan::F17},
          {sf::Keyboard::Scan::F18},
          {sf::Keyboard::Scan::F19},
          {sf::Keyboard::Scan::F20},
          {sf::Keyboard::Scan::F21},
          {sf::Keyboard::Scan::F22},
          {sf::Keyboard::Scan::F23},
          {sf::Keyboard::Scan::F24}}},
        {{{sf::Keyboard::Scan::Application},
          {sf::Keyboard::Scan::Execute},
          {sf::Keyboard::Scan::ModeChange},
          {sf::Keyboard::Scan::Help},
          {sf::Keyboard::Scan::Select},
          {sf::Keyboard::Scan::Redo},
          {sf::Keyboard::Scan::Undo},
          {sf::Keyboard::Scan::Cut},
          {sf::Keyboard::Scan::Copy},
          {sf::Keyboard::Scan::Paste},
          {sf::Keyboard::Scan::VolumeMute},
          {sf::Keyboard::Scan::VolumeUp},
          {sf::Keyboard::Scan::VolumeDown},
          {sf::Keyboard::Scan::MediaPlayPause},
          {sf::Keyboard::Scan::MediaStop},
          {sf::Keyboard::Scan::MediaNextTrack},
          {sf::Keyboard::Scan::MediaPreviousTrack}}},
        {{{sf::Keyboard::Scan::Back},
          {sf::Keyboard::Scan::Forward},
          {sf::Keyboard::Scan::Refresh},
          {sf::Keyboard::Scan::Stop},
          {sf::Keyboard::Scan::Search},
          {sf::Keyboard::Scan::Favorites},
          {sf::Keyboard::Scan::HomePage},
          {sf::Keyboard::Scan::LaunchApplication1},
          {sf::Keyboard::Scan::LaunchApplication2},
          {sf::Keyboard::Scan::LaunchMail},
          {sf::Keyboard::Scan::LaunchMediaSelect}}},
    }};

    sf::VertexArray                                m_triangles;
    std::vector<sf::Text>                          m_labels;
    std::array<float, sf::Keyboard::ScancodeCount> m_bloatFactor;
};

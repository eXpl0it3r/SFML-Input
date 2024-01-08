#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <type_traits>

template <typename Enum>
class EnumRange
{
public:
    class Iterator
    {
    public:
        constexpr explicit Iterator(std::underlying_type_t<Enum> value) : m_value{value}
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

    constexpr explicit EnumRange(std::underlying_type_t<Enum> begin, std::underlying_type_t<Enum> end) :
    m_begin{begin},
    m_end{end}
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
    const std::underlying_type_t<Enum> m_begin, m_end;
};

constexpr auto keys      = EnumRange<sf::Keyboard::Key>{0, sf::Keyboard::KeyCount};
constexpr auto scancodes = EnumRange<sf::Keyboard::Scancode>{0, sf::Keyboard::ScancodeCount};
constexpr auto buttons   = EnumRange<sf::Mouse::Button>{0, sf::Mouse::ButtonCount};

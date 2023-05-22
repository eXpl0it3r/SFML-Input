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

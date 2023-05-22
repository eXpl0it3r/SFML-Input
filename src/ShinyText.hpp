#pragma once

#include <SFML/Graphics/Text.hpp>

#include <SFML/System/Time.hpp>

class ShinyText : public sf::Text
{
public:
    using sf::Text::Text;

    void shine(const sf::Color& color = sf::Color::Yellow);
    void update(sf::Time frameTime);

private:
    static inline const sf::Time m_duration = sf::milliseconds(150);
    sf::Time                     m_remaining;
};

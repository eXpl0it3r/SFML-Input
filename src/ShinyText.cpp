#include "ShinyText.hpp"

void ShinyText::shine(const sf::Color& color)
{
    setOutlineColor(color);
    m_remaining = m_duration;
}

void ShinyText::update(sf::Time frameTime)
{
    const float ratio = m_remaining / m_duration;
    const float alpha = std::max(0.f, ratio * (2.f - ratio)) * 0.5f;

    auto color = getOutlineColor();
    color.a    = 255 * alpha;
    setOutlineColor(color);

    if (sf::Time::Zero < m_remaining)
        m_remaining -= frameTime;
}

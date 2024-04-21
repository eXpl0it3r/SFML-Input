#include "KeyboardView.hpp"

#include <unordered_set>

#include <cassert>
#include <cmath>

KeyboardView::KeyboardView(const sf::Font& font) :
m_triangles{sf::PrimitiveType::Triangles, sf::Keyboard::ScancodeCount * 6},
m_labels(sf::Keyboard::ScancodeCount, sf::Text{font, "", 16})
{
    m_bloatFactor.fill(1.f);

    // Check all the scancodes are in the matrix exactly once
    {
        auto scancodesInMatrix = std::unordered_set<sf::Keyboard::Scancode>{};
        for (const auto& [cells, marginBottom] : matrix)
        {
            for (const auto& [scancode, size, marginRight] : cells)
            {
                assert(scancodesInMatrix.count(scancode) == 0);
                scancodesInMatrix.insert(scancode);
            }
        }
        assert(scancodesInMatrix.size() == sf::Keyboard::ScancodeCount);
    }

    // Initialize labels
    auto position = sf::Vector2f{};
    for (const auto& [cells, marginBottom] : matrix)
    {
        for (const auto& [scancode, size, marginRight] : cells)
        {
            auto& label = m_labels[static_cast<std::size_t>(scancode)];
            label.setString(sf::Keyboard::getDescription(scancode));
            label.setPosition(position + size / 2.f);

            if (size.x < label.getLocalBounds().width + padding * 2.f + 2.f)
            {
                auto string = label.getString();
                string.replace(" ", "\n");
                label.setString(string);
            }
            while (size.x < label.getLocalBounds().width + padding * 2.f + 2.f)
                label.setCharacterSize(label.getCharacterSize() - 2);

            const auto bounds = label.getLocalBounds();
            label.setOrigin(sf::Vector2f{std::round(bounds.left + bounds.width / 2.f),
                                         std::round(static_cast<float>(label.getCharacterSize()) / 2.f)});

            position.x += size.x + marginRight;
        }
        position.x = 0.f;
        position.y += keySize + marginBottom;
    }
}

void KeyboardView::handle(const sf::Event& event)
{
    if (const auto* keyPressedEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressedEvent->scancode != sf::Keyboard::Scan::Unknown)
            m_bloatFactor[static_cast<std::size_t>(keyPressedEvent->scancode)] = 0.5f;
    }
    else if (const auto* keyReleasedEvent = event.getIf<sf::Event::KeyReleased>())
    {
        if (keyReleasedEvent->scancode != sf::Keyboard::Scan::Unknown)
            m_bloatFactor[static_cast<std::size_t>(keyReleasedEvent->scancode)] = 1.5f;
    }
}

void KeyboardView::update(sf::Time frameTime)
{
    const auto transitionDuration = sf::seconds(0.3f);
    for (auto& zoom : m_bloatFactor)
    {
        const auto absBloatChange = std::min(std::abs(zoom - 1.f), frameTime / transitionDuration);
        zoom += 1.f < zoom ? -absBloatChange : absBloatChange;
    }

    const auto square = std::vector<sf::Vector2f>{
        {0.f, 0.f},
        {1.f, 0.f},
        {1.f, 1.f},
        {0.f, 1.f},
    };
    constexpr auto indexes = std::array{0, 1, 3, 3, 1, 2};

    auto position = sf::Vector2f{};
    for (const auto& [cells, marginBottom] : matrix)
    {
        for (const auto& [scancode, size, marginRight] : cells)
        {
            const auto pressed = sf::Keyboard::isKeyPressed(scancode);
            const auto pad     = padding - padding * (m_bloatFactor[static_cast<std::size_t>(scancode)] - 1.f);
            for (const auto index : {0, 1, 2, 3, 4, 5})
            {
                const auto& corner = square[indexes[index]];
                auto&       vertex = m_triangles[6 * static_cast<int>(scancode) + index];

                vertex.position = position + sf::Vector2f{pad + (size.x - 2.f * pad) * corner.x,
                                                          pad + (size.y - 2.f * pad) * corner.y};
                vertex.color    = pressed ? sf::Color{96, 96, 96} : sf::Color{48, 48, 48};
            }
            position.x += size.x + marginRight;
        }
        position.x = 0.f;
        position.y += keySize + marginBottom;
    }
}

void KeyboardView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_triangles, states);
    for (const auto& label : m_labels)
        target.draw(label, states);
}

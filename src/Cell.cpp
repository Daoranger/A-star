//
// Created by hoang on 2/13/2026.
//

#include "Cell.h"

Cell::Cell()
    : m_bBlocked(false)
{
}

Cell::Cell(const float cellSize)
    : m_bBlocked(false)
{
    m_square.setSize(sf::Vector2f(cellSize, cellSize));
    m_square.setFillColor(sf::Color::White);
    constexpr auto grey = sf::Color(49, 49, 49);
    m_square.setOutlineColor(grey);
    m_square.setOutlineThickness(5);
}

void Cell::draw(sf::RenderWindow& window)
{
    if (getbBlocked())
        m_square.setFillColor(sf::Color::Black);
    else
        m_square.setFillColor(sf::Color::White);

    window.draw(m_square);
}

void Cell::setSquarePosition(const sf::Vector2f &pos)
{
    m_square.setPosition(pos);
}

void Cell::setbBlocked(const bool bBlocked)
{
    m_bBlocked = bBlocked;
}

bool Cell::getbBlocked() const
{
    return m_bBlocked;
}

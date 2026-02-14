//
// Created by hoang on 2/13/2026.
//

#include "Cell.h"

Cell::Cell()
    : m_bBlocked{false}
{
    m_square.setSize(sf::Vector2f(50, 50));
    m_square.setOutlineColor(sf::Color::Black);
    m_square.setOutlineThickness(5);
}

void Cell::draw(sf::RenderWindow& window) const
{
    window.draw(m_square);
}

void Cell::setSquarePosition(const sf::Vector2f &pos)
{
    m_square.setPosition(pos);
}


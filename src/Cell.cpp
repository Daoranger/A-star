//
// Created by hoang on 2/13/2026.
//

#include "Cell.h"

Cell::Cell()
{
    m_square.setSize(sf::Vector2f(50, 50));
}

void Cell::draw(sf::RenderWindow& window) const
{
    window.draw(m_square);
}

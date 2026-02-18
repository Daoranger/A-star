//
// Created by hoang on 2/13/2026.
//

#include "Cell.h"

Cell::Cell()
    : m_cellType(CellType::open)
{
}

Cell::Cell(const float cellSize)
    : m_cellType(CellType::open)
{
    m_square.setSize(sf::Vector2f(cellSize, cellSize));
    constexpr auto grey = sf::Color(49, 49, 49);
    m_square.setOutlineColor(grey);
    m_square.setOutlineThickness(5);
}

void Cell::draw(sf::RenderWindow& window)
{
    switch (m_cellType)
    {
        case CellType::start:
            m_square.setFillColor(sf::Color::Green);
            break;
        case CellType::goal:
            m_square.setFillColor(sf::Color::Red);
            break;
        case CellType::obstacle:
            m_square.setFillColor(sf::Color::Black);
            break;
        case CellType::open:
            m_square.setFillColor(sf::Color::White);
            break;
    }

    window.draw(m_square);
}

void Cell::setSquarePosition(const sf::Vector2f &pos)
{
    m_square.setPosition(pos);
}

void Cell::setCellType(CellType cellType)
{
    m_cellType = cellType;
}

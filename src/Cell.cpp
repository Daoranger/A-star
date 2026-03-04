//
// Created by hoang on 2/13/2026.
//

#include "Cell.h"

Cell::Cell(const float cellSize, int x, int y)
    : m_cellType(CellType::open)
    , m_x(x)
    , m_y(y)
    , m_g(0.0)
    , m_h(0.0)
    , m_f(0.0)
    , m_parent(nullptr)
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

CellType Cell::getCellType() const
{
    return m_cellType;
}

bool Cell::operator>(const Cell &other) const
{
    return m_f > other.m_f;
}

bool Cell::operator==(const Cell& other)
{
    return m_x == other.m_x && m_y == other.m_y;
}

std::ostream& operator<<(std::ostream &out, const Cell& cell)
{
    out << "Cell(row=" << cell.m_x << ", col=" << cell.m_y << ")\n";
    return out;
}

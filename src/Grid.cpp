//
// Created by hoang on 2/13/2026.
//

#include "Grid.h"
#include <iostream>

Grid::Grid()
    : m_rows{ 50 }
    , m_cols{ 50 }
    , m_grid(m_rows, std::vector<Cell>(m_cols))
{
}

void Grid::draw(sf::RenderWindow &window)
{
    for (std::size_t row = 0; row < m_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            m_grid[row][col].setSquarePosition(sf::Vector2f(static_cast<float>(50 * row), static_cast<float>(50 * col)));
            m_grid[row][col].draw(window);
        }
    }
}

std::size_t Grid::getRows()
{
    return m_rows;
}

std::size_t Grid::getCols()
{
    return m_cols;
}

void Grid::toggleCellBlocked(std::size_t row, std::size_t col)
{
    std::cout << "row=" << row << ", col=" << col << '\n';
    m_grid[row][col].setbBlocked(!m_grid[row][col].getbBlocked());
}


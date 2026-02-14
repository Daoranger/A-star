//
// Created by hoang on 2/13/2026.
//

#include "Grid.h"

Grid::Grid()
    : m_rows{ 20 }
    , m_cols{ 20 }
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

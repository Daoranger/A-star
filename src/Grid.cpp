//
// Created by hoang on 2/13/2026.
//

#include "Grid.h"
#include <iostream>

Grid::Grid()
    : m_rows(20)
    , m_cols(20)
    , m_cellSize(50)
    , m_grid(m_rows, std::vector<Cell>(m_cols, Cell(m_cellSize)))
{
}

Grid::Grid(std::size_t rows, std::size_t cols, float cellSize)
    : m_rows(rows)
    , m_cols(cols)
    , m_cellSize(cellSize)
    , m_grid(m_rows, std::vector<Cell>(m_cols, Cell(m_cellSize)))
{
}

void Grid::draw(sf::RenderWindow &window)
{
    for (std::size_t row = 0; row < m_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            m_grid[row][col].setSquarePosition(sf::Vector2f(static_cast<float>(m_cellSize * row), static_cast<float>(m_cellSize * col)));
            m_grid[row][col].draw(window);
        }
    }
}

std::size_t Grid::getRows() const
{
    return m_rows;
}

std::size_t Grid::getCols() const
{
    return m_cols;
}

float Grid::getCellSize() const
{
    return m_cellSize;
}

void Grid::toggleCellBlocked(std::size_t row, std::size_t col)
{
    std::cout << "row=" << row << ", col=" << col << '\n';
    m_grid[row][col].setbBlocked(!m_grid[row][col].getbBlocked());
}


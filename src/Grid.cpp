//
// Created by hoang on 2/13/2026.
//

#include "Grid.h"
#include <iostream>

Grid::Grid(std::size_t rows, std::size_t cols, float cellSize)
    : m_rows(rows)
    , m_cols(cols)
    , m_cellSize(cellSize)
    , m_cells(m_rows, std::vector<Cell>(m_cols, Cell(m_cellSize)))
    , m_startCell(nullptr)
    , m_goalCell(nullptr)
{
}

void Grid::draw(sf::RenderWindow &window)
{
    for (std::size_t row = 0; row < m_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            m_cells[row][col].setSquarePosition(sf::Vector2f(static_cast<float>(m_cellSize * row), static_cast<float>(m_cellSize * col)));
            m_cells[row][col].draw(window);
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
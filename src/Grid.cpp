//
// Created by hoang on 2/13/2026.
//

#include "Grid.h"
#include <iostream>

Grid::Grid(std::size_t rows, std::size_t cols, float cellSize)
    : m_rows(rows)
    , m_cols(cols)
    , m_cellSize(cellSize)
    , m_cells(m_rows, std::vector<Cell>(m_cols, Cell(m_cellSize, 0, 0)))
    , m_startCell(nullptr)
    , m_goalCell(nullptr)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            m_cells[i][j].m_x = i;
            m_cells[i][j].m_y = j;
        }
    }
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

std::vector<Cell> Grid::astar()
{
    // open list is a prioriy queue which priortize the smaller value,
    // which turn it into a min-heap
    std::priority_queue<Cell, std::vector<Cell>, std::greater<Cell>> openList;
    openList.push(*m_startCell);

    std::vector<Cell> closedList;

    m_startCell->m_g = 0;
    m_startCell->m_h = heuristic(*m_startCell, *m_startCell);
    m_startCell->m_f = m_startCell->m_g + m_startCell->m_h;
    m_startCell->m_parent = nullptr;


    while (!openList.empty())
    {
        Cell currCell = openList.top();

        if (currCell == *m_goalCell)
        {
            std::vector<Cell> path;

            while (currCell.m_parent)
            {
                path.push_back(currCell);
                currCell = *currCell.m_parent;
            }
            return path;
        }

        // Once we're done with the current node,
        // remove it from open list, and add it to closed list.
        closedList.push_back(openList.top());
        openList.pop();

        std::cout << "Current cell: " << currCell << "\n";
    }

    return {};
}

double Grid::heuristic(const Cell &currCell, const Cell &goalCell)
{
    double dx = currCell.m_x - goalCell.m_x;
    double dy = currCell.m_y - goalCell.m_y;

    return std::sqrt((dx * dx) + (dy * dy));
}

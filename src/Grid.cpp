//
// Created by hoang on 2/13/2026.
//

#include "Grid.h"
#include "Snapshot.h"
#include <iostream>
#include <unordered_set>
#include <set>

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
    sf::Vector2f offset(
        window.getSize().x / 2.f - m_cellSize * m_rows / 2.f,
        window.getSize().y / 2.f - m_cellSize * m_cols / 2.f
    );

    for (std::size_t row = 0; row < m_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            m_cells[row][col].setSquarePosition(
                sf::Vector2f(row * m_cellSize, col * m_cellSize) + offset
            );
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

std::vector<Cell*> Grid::astar(std::vector<Snapshot>& snapshots, Snapshot& snapshot)
{
    std::set<Cell*, CompareCell> openSet;
    std::unordered_set<Cell*> closedSet;

    m_startCell->m_g = 0;
    m_startCell->m_h = heuristic(*m_startCell, *m_goalCell);
    m_startCell->m_f = m_startCell->m_g + m_startCell->m_h;
    m_startCell->m_parent = nullptr;

    // add start cell to openList
    openSet.insert(m_startCell);

    while (!openSet.empty())
    {
        // current cell = node with lowest m_f in openList
        Cell* currCell = *openSet.begin();

        // if current cell is goal, reconstruct and return path
        if (*currCell == *m_goalCell)
        {
            std::vector<Cell*> path;

            while (currCell != nullptr)
            {
                path.push_back(currCell);
                currCell = currCell->m_parent;
            }
            // change from goal-to-start order to start-to-goal order

            return path;
        }

        // remove current cell from openList
        openSet.erase(currCell);

        // add current cell to closedList
        closedSet.insert(currCell);

        // create new snapshot and push to the list of snapshots
        snapshot.m_openVector.push_back(currCell);
        snapshots.push_back(snapshot);

        // for each neighbor of current cell
        for (auto& n : getValidNeighbors(*currCell))
        {
            Cell* neighborCell = &m_cells[n.first][n.second];

            // if neighbor in closedList
            if (closedSet.find(neighborCell) != closedSet.end())
            {
                continue;
            }

            // the distance from start to a neighbor, if we follow current path
            // because I am using a uniform grid where you can only move to adjacent cells,
            // the edge cost between any two neighbors is always 1
            double tentative_g = currCell->m_g + 1.0;

            // if neighbor is not in openList or this path to neighbor cell is better than any previous one
            // this mean path from start to current cell cost less than path from start to any previous one
            if (openSet.find(neighborCell) == openSet.end() || tentative_g < neighborCell->m_g)
            {
                openSet.erase(neighborCell);
                neighborCell->m_parent = currCell;
                neighborCell->m_g = tentative_g;
                neighborCell->m_h = heuristic(*neighborCell, *m_goalCell);
                neighborCell->m_f = neighborCell->m_g + neighborCell->m_h;

                if (openSet.find(neighborCell) == openSet.end())
                {
                    openSet.insert(neighborCell);
                    snapshot.m_closedVector.push_back(neighborCell);
                    snapshots.push_back(snapshot);
                }
            }
        }
    }
    // failure no path exists, return empty path;
    return {};
}

double Grid::heuristic(const Cell &currCell, const Cell &goalCell)
{
    // Manhattan distance
    double dx = std::abs(currCell.m_x - goalCell.m_x);
    double dy = std::abs(currCell.m_y - goalCell.m_y);
    return dx + dy;
}

std::vector<std::pair<int, int>> Grid::getValidNeighbors(const Cell &currCell)
{
    int currCellX = currCell.m_x;
    int currCellY = currCell.m_y;

    // pair<int, int>: first = row, second = col
    std::vector<std::pair<int, int>> possible_neighbors {
        {currCellX - 1, currCellY},
        {currCellX + 1, currCellY},
        {currCellX, currCellY - 1},
        {currCellX, currCellY + 1}
    };

    std::vector<std::pair<int, int>> valid_neighbors {};

    for (auto& pn : possible_neighbors)
    {
        if ((pn.first >= 0 && pn.second >=0) && (pn.first < m_rows && pn.second < m_cols) && (m_cells[pn.first][pn.second].getCellType() != CellType::obstacle))
        {
            valid_neighbors.push_back(pn);
        }
    }
    return valid_neighbors;
}

void Grid::resetCells()
{
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            if (m_cells[i][j].getCellType() == CellType::path)
            {
                m_cells[i][j].resetCell();
            }
        }
    }
}

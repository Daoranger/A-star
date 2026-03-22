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

std::vector<Cell*> Grid::astar()
{
    // open list is a prioriy queue which priortize the smaller value,
    // which turn it into a min-heap
    std::priority_queue<Cell*, std::vector<Cell*>, CompareCell>  openList;
    openList.push(m_startCell);

    // use only to keep track of membership in openList (cuz priority_queue doesn't support iterators)
    std::vector<Cell*> openListMembership;
    openListMembership.push_back(m_startCell);

    std::vector<Cell*> closedList;

    m_startCell->m_g = 0;
    m_startCell->m_h = heuristic(*m_startCell, *m_goalCell);
    m_startCell->m_f = m_startCell->m_g + m_startCell->m_h;
    m_startCell->m_parent = nullptr;


    while (!openList.empty())
    {
        std::cout << "List not empty\n";
        Cell* currCell = openList.top();

        // check if we've reached goalCell
        // if yes, return the path;
        if (*currCell == *m_goalCell)
        {
            std::vector<Cell*> path;

            while (currCell->m_parent)
            {
                path.push_back(currCell);
                currCell = currCell->m_parent;
            }
            return path;
        }

        // Once we're done with the current node,
        // remove it from open list, and add it to closed list.
        closedList.push_back(openList.top());
        openList.pop();
        openListMembership.pop_back();

        //std::cout << "Current cell: " << currCell << "\n";

        std::vector<std::pair<int, int>> neighbors = getValidNeighbors(*currCell);

        for (auto& n : neighbors)
        {
            Cell* neighborCell = &m_cells[n.first][n.second];

            // if neighbor in closedList
            if (std::count(closedList.begin(), closedList.end(), neighborCell) > 0)
            {
                continue;   // skip already evaluated Cells
            }

            int tentative_g = currCell->m_g + heuristic(*currCell, *neighborCell);

            // if neighbor is not in openList
            if (std::count(openListMembership.begin(), openListMembership.end(), neighborCell) <= 0)
            {
                openList.push(neighborCell);
                openListMembership.push_back(neighborCell);
            }
            else if (tentative_g >= neighborCell->m_g)
            {
                continue;
            }

            neighborCell->m_parent = currCell;
            neighborCell->m_g = tentative_g;
            neighborCell->m_h = heuristic(*neighborCell, *m_goalCell);
            neighborCell->m_f = neighborCell->m_g + neighborCell->m_h;
        }
    }

    // failure no path exists, return empty path;
    std::cout << "No path\n";
    return {};
}

double Grid::heuristic(const Cell &currCell, const Cell &goalCell)
{

    // Euclidian distance
    double dx = currCell.m_x - goalCell.m_x;
    double dy = currCell.m_y - goalCell.m_y;

    return std::sqrt((dx * dx) + (dy * dy));
}

std::vector<std::pair<int, int>> Grid::getValidNeighbors(const Cell &currCell)
{
    int currCellX = currCell.m_x;
    int currCellY = currCell.m_y;

    // pair<int, int>: first = row, second = row
    std::vector<std::pair<int, int>> possible_neighbors {
        {currCellX - 1, currCellY - 1},
        {currCellX - 1, currCellY},
        {currCellX - 1, currCellY + 1},
        {currCellX, currCellY - 1},
        {currCellX, currCellY + 1},
        {currCellX + 1, currCellY - 1},
        {currCellX + 1, currCellY},
        {currCellX + 1, currCellY + 1}
    };

    std::vector<std::pair<int, int>> valid_neighbors {};

    for (auto& pn : possible_neighbors)
    {
        if ((pn.first >= 0 && pn.second >=0) && (pn.first < m_rows && pn.second < m_cols))
        {
            valid_neighbors.push_back(pn);
        }
    }

    // DEBUG: print valid_neighbors
    // for (auto& vn : valid_neighbors)
    // {
    //     std::cout << vn.first << " " << vn.second << "\n";
    // }

    return valid_neighbors;
}

void Grid::resetCells()
{
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            if (m_cells[i][j].getCellType() == CellType::start || m_cells[i][j].getCellType() == CellType::goal)
            {
                continue;
            }

            m_cells[i][j].resetCell();
        }
    }
}

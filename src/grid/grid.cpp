//
// Created by hoang on 2/13/2026.
//

#include "grid.h"
#include "../pathfinding/Snapshot.h"
#include <iostream>

Grid::Grid(std::size_t rows, std::size_t cols, float cellSize)
    : rows_(rows)
    , cols_(cols)
    , cell_size_(cellSize)
    , cells_(rows_, std::vector<Cell>(cols_, Cell(cell_size_, 0, 0)))
    , start_cell_(nullptr)
    , goal_cell_(nullptr)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            cells_[i][j].x_ = i;
            cells_[i][j].y_ = j;
        }
    }
}

void Grid::draw(sf::RenderWindow &window)
{
    sf::Vector2f offset(
        window.getSize().x / 2.f - cell_size_ * rows_ / 2.f,
        window.getSize().y / 2.f - cell_size_ * cols_ / 2.f
    );

    for (std::size_t row = 0; row < rows_; ++row)
    {
        for (std::size_t col = 0; col < cols_; ++col)
        {
            cells_[row][col].setPosition(
                sf::Vector2f(row * cell_size_, col * cell_size_) + offset
            );
            cells_[row][col].draw(window);
        }
    }
}

std::size_t Grid::getRows() const
{
    return rows_;
}

std::size_t Grid::getCols() const
{
    return cols_;
}

float Grid::getCellSize() const
{
    return cell_size_;
}

std::vector<Cell*> Grid::astar(std::vector<Snapshot>& snapshots, Snapshot& snapshot, std::size_t& nodesExpanded)
{
    std::set<Cell*, CompareCell> openSet;
    std::unordered_set<Cell*> closedSet;

    start_cell_->g_ = 0;
    start_cell_->h_ = heuristic(*start_cell_, *goal_cell_);
    start_cell_->f_ = start_cell_->g_ + start_cell_->h_;
    start_cell_->parent_ = nullptr;

    // add start cell to openList
    openSet.insert(start_cell_);
    snapshot.m_openVector = extractNodes(openSet);
    snapshot.m_closedVector = extractNodes(closedSet);
    snapshots.push_back(snapshot);

    while (!openSet.empty())
    {
        // current cell = node with lowest m_f in openList
        Cell* currCell = *openSet.begin();

        // if current cell is goal, reconstruct and return path
        if (*currCell == *goal_cell_)
        {
            std::vector<Cell*> path;

            while (currCell != nullptr)
            {
                path.push_back(currCell);
                currCell = currCell->parent_;
            }
            // change from goal-to-start order to start-to-goal order
            std::reverse(path.begin(), path.end());
            nodesExpanded = closedSet.size() + 1;
            return path;
        }

        // remove current cell from openList
        openSet.erase(currCell);

        // add current cell to closedList
        closedSet.insert(currCell);

        snapshot.m_openVector = extractNodes(openSet);
        snapshot.m_closedVector = extractNodes(closedSet);
        snapshots.push_back(snapshot);

        // for each neighbor of current cell
        for (auto& n : getValidNeighbors(*currCell))
        {
            Cell* neighborCell = &cells_[n.first][n.second];

            // if neighbor in closedList
            if (closedSet.find(neighborCell) != closedSet.end())
            {
                continue;
            }

            // the distance from start to a neighbor, if we follow current path
            // because I am using a uniform grid where you can only move to adjacent cells,
            // the edge cost between any two neighbors is always 1
            double tentative_g = currCell->g_ + 1.0;

            // if neighbor is not in openList or this path to neighbor cell is better than any previous one
            // this mean path from start to current cell cost less than path from start to any previous one
            if (openSet.find(neighborCell) == openSet.end() || tentative_g < neighborCell->g_)
            {
                openSet.erase(neighborCell);
                neighborCell->parent_ = currCell;
                neighborCell->g_ = tentative_g;
                neighborCell->h_ = heuristic(*neighborCell, *goal_cell_);
                neighborCell->f_ = neighborCell->g_ + neighborCell->h_;
                openSet.insert(neighborCell);

                snapshot.m_openVector = extractNodes(openSet);
                snapshot.m_closedVector = extractNodes(closedSet);
                snapshots.push_back(snapshot);
            }
        }
    }
    // failure no path exists, return empty path;
    nodesExpanded = closedSet.size();
    return {};
}

double Grid::heuristic(const Cell &currCell, const Cell &goalCell)
{
    // Manhattan distance
    double dx = std::abs(currCell.x_ - goalCell.x_);
    double dy = std::abs(currCell.y_ - goalCell.y_);
    return (dx + dy) * 1.001; // tiny nudge breaks ties
}

std::vector<std::pair<int, int>> Grid::getValidNeighbors(const Cell &currCell)
{
    int currCellX = currCell.x_;
    int currCellY = currCell.y_;

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
        if ((pn.first >= 0 && pn.second >=0) && (pn.first < rows_ && pn.second < cols_) && (cells_[pn.first][pn.second].getType() != CellType::obstacle))
        {
            valid_neighbors.push_back(pn);
        }
    }
    return valid_neighbors;
}

void Grid::resetCells()
{
    for (int i = 0; i < rows_; ++i)
    {
        for (int j = 0; j < cols_; ++j)
        {
            if (cells_[i][j].getType() == CellType::path)
            {
                cells_[i][j].reset();
            }
        }
    }
}

std::vector<Cell*> Grid::extractNodes(const std::set<Cell*, CompareCell>& set)
{
    return std::vector<Cell*>(set.begin(), set.end());
}

std::vector<Cell*> Grid::extractNodes(const std::unordered_set<Cell*>& unordered_set)
{
    return std::vector<Cell*>(unordered_set.begin(), unordered_set.end());
}



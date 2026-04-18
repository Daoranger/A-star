//
// Created by hoang on 4/2/2026.
//

#include "agent.h"

#include <algorithm>

#include "../grid/grid.h"

Agent::Agent(Cell* start_cell, Cell* goal_cell, const Grid& grid, sf::Color color, bool record_search_snapshots)
    : start_cell_(start_cell)
    , goal_cell_(goal_cell)
    , grid_(grid)
    , color_(color)
    , record_search_snapshots_(record_search_snapshots)
{
}

void Agent::runAStar()
{
    snapshots_.clear();
    snapshot_index_ = 0;
    path_.clear();

    Snapshot snapshot;
    std::set<Cell*, CompareCell> openSet(CompareCell{f_});
    std::unordered_set<Cell*> closedSet;

    // start cell
    g_[start_cell_] = 0;
    h_[start_cell_] = heuristic(*start_cell_, *goal_cell_);
    f_[start_cell_] = g_.at(start_cell_) + h_.at(start_cell_);
    parent_[start_cell_] = nullptr;


    // add start cell to openList
    openSet.insert(start_cell_);
    if (record_search_snapshots_)
    {
        snapshot.frontier_ = extractNodes(openSet);
        snapshot.explored_ = extractNodes(closedSet);
        snapshots_.push_back(snapshot);
    }

    // processing current cell in open set
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
                currCell = parent_.at(currCell);
            }
            // change from goal-to-start order to start-to-goal order
            std::reverse(path.begin(), path.end());
            path_ = path;
            metrics_.nodes_expanded = closedSet.size() + 1;
            return;
        }

        // remove current cell from openList
        openSet.erase(currCell);

        // add current cell to closedList
        closedSet.insert(currCell);

        if (record_search_snapshots_)
        {
            snapshot.frontier_ = extractNodes(openSet);
            snapshot.explored_ = extractNodes(closedSet);
            snapshots_.push_back(snapshot);
        }

        // for each neighbor of current cell
        for (auto& n : getValidNeighbors(*currCell))
        {
            Cell* neighborCell = const_cast<Cell*>(&grid_.cells_[n.first][n.second]);

            // if neighbor in closedList
            if (closedSet.find(neighborCell) != closedSet.end())
            {
                continue;
            }

            // the distance from start to a neighbor, if we follow current path
            // because I am using a uniform grid where you can only move to adjacent cells,
            // the edge cost between any two neighbors is always 1
            double tentative_g = g_.at(currCell) + 1.0;

            // if neighbor is not in openList or this path to neighbor cell is better than any previous one
            // this mean path from start to current cell cost less than path from start to any previous one
            if (openSet.find(neighborCell) == openSet.end() || tentative_g < g_.at(neighborCell))
            {
                openSet.erase(neighborCell);
                parent_[neighborCell] = currCell;
                g_[neighborCell] = tentative_g;
                h_[neighborCell] = heuristic(*neighborCell, *goal_cell_);
                f_[neighborCell] = g_.at(neighborCell) + h_.at(neighborCell);
                openSet.insert(neighborCell);
            }
        }
    }

    // no path found
    metrics_.nodes_expanded = closedSet.size();
}

void Agent::runDijkstra()
{
    snapshots_.clear();
    snapshot_index_ = 0;
    path_.clear();

    Snapshot snapshot;
    std::set<Cell*, CompareCell> openSet(CompareCell{f_});
    std::unordered_set<Cell*> closedSet;

    // start cell
    g_[start_cell_] = 0;
    f_[start_cell_] = g_.at(start_cell_);
    parent_[start_cell_] = nullptr;


    // add start cell to openList
    openSet.insert(start_cell_);
    if (record_search_snapshots_)
    {
        snapshot.frontier_ = extractNodes(openSet);
        snapshot.explored_ = extractNodes(closedSet);
        snapshots_.push_back(snapshot);
    }

    // processing current cell in open set
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
                currCell = parent_.at(currCell);
            }
            // change from goal-to-start order to start-to-goal order
            std::reverse(path.begin(), path.end());
            path_ = path;
            metrics_.nodes_expanded = closedSet.size() + 1;
            return;
        }

        // remove current cell from openList
        openSet.erase(currCell);

        // add current cell to closedList
        closedSet.insert(currCell);

        if (record_search_snapshots_)
        {
            snapshot.frontier_ = extractNodes(openSet);
            snapshot.explored_ = extractNodes(closedSet);
            snapshots_.push_back(snapshot);
        }

        // for each neighbor of current cell
        for (auto& n : getValidNeighbors(*currCell))
        {
            Cell* neighborCell = const_cast<Cell*>(&grid_.cells_[n.first][n.second]);

            // if neighbor in closedList
            if (closedSet.find(neighborCell) != closedSet.end())
            {
                continue;
            }

            // the distance from start to a neighbor, if we follow current path
            // because I am using a uniform grid where you can only move to adjacent cells,
            // the edge cost between any two neighbors is always 1
            double tentative_g = g_.at(currCell) + 1.0;

            // if neighbor is not in openList or this path to neighbor cell is better than any previous one
            // this mean path from start to current cell cost less than path from start to any previous one
            if (openSet.find(neighborCell) == openSet.end() || tentative_g < g_.at(neighborCell))
            {
                openSet.erase(neighborCell);
                parent_[neighborCell] = currCell;
                g_[neighborCell] = tentative_g;
                f_[neighborCell] = g_.at(neighborCell);
                openSet.insert(neighborCell);
            }
        }
    }

    // no path found
    metrics_.nodes_expanded = closedSet.size();
}

void Agent::runGreedy()
{
    snapshots_.clear();
    snapshot_index_ = 0;
    path_.clear();

    Snapshot snapshot;
    std::set<Cell*, CompareCell> openSet(CompareCell{f_});
    std::unordered_set<Cell*> closedSet;

    // start cell
    h_[start_cell_] = heuristic(*start_cell_, *goal_cell_);
    f_[start_cell_] = h_.at(start_cell_);
    parent_[start_cell_] = nullptr;


    // add start cell to openList
    openSet.insert(start_cell_);
    if (record_search_snapshots_)
    {
        snapshot.frontier_ = extractNodes(openSet);
        snapshot.explored_ = extractNodes(closedSet);
        snapshots_.push_back(snapshot);
    }

    // processing current cell in open set
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
                currCell = parent_.at(currCell);
            }
            // change from goal-to-start order to start-to-goal order
            std::reverse(path.begin(), path.end());
            path_ = path;
            metrics_.nodes_expanded = closedSet.size() + 1;
            return;
        }

        // remove current cell from openList
        openSet.erase(currCell);

        // add current cell to closedList
        closedSet.insert(currCell);

        if (record_search_snapshots_)
        {
            snapshot.frontier_ = extractNodes(openSet);
            snapshot.explored_ = extractNodes(closedSet);
            snapshots_.push_back(snapshot);
        }

        // for each neighbor of current cell
        for (auto& n : getValidNeighbors(*currCell))
        {
            Cell* neighborCell = const_cast<Cell*>(&grid_.cells_[n.first][n.second]);

            // if neighbor in closedList
            if (closedSet.find(neighborCell) != closedSet.end())
            {
                continue;
            }

            // if neighbor is not in openList or this path to neighbor cell is better than any previous one
            // this mean path from start to current cell cost less than path from start to any previous one
            if (openSet.find(neighborCell) == openSet.end())
            {
                openSet.erase(neighborCell);
                parent_[neighborCell] = currCell;
                h_[neighborCell] = heuristic(*neighborCell, *goal_cell_);
                f_[neighborCell] = h_.at(neighborCell);
                openSet.insert(neighborCell);
            }
        }
    }

    // no path found
    metrics_.nodes_expanded = closedSet.size();
}

void Agent::runBFS()
{
    snapshots_.clear();
    snapshot_index_ = 0;
    path_.clear();

    Snapshot snapshot;
    std::deque<Cell*> openDeque;
    std::unordered_set<Cell*> visitedSet;

    // start cell
    parent_[start_cell_] = nullptr;
    openDeque.push_back(start_cell_);
    visitedSet.insert(start_cell_);

    if (record_search_snapshots_)
    {
        snapshot.frontier_ = extractNodes(openDeque);
        snapshot.explored_ = extractNodes(visitedSet);
        snapshots_.push_back(snapshot);
    }

    // processing current cell in open set
    while (!openDeque.empty())
    {
        Cell* currCell = openDeque.front();
        openDeque.pop_front();

        // if current cell is goal, reconstruct and return path
        if (*currCell == *goal_cell_)
        {
            std::vector<Cell*> path;

            while (currCell != nullptr)
            {
                path.push_back(currCell);
                currCell = parent_.at(currCell);
            }
            // change from goal-to-start order to start-to-goal order
            std::reverse(path.begin(), path.end());
            path_ = path;
            metrics_.nodes_expanded = visitedSet.size() + 1;
            return;
        }

        // for each neighbor of current cell
        for (auto& n : getValidNeighbors(*currCell))
        {
            Cell* neighborCell = const_cast<Cell*>(&grid_.cells_[n.first][n.second]);

            // if neighbor in closedList (we have visited)
            if (visitedSet.find(neighborCell) != visitedSet.end())
            {
                continue;
            }

            // if we haven't visited the cell
            visitedSet.insert(neighborCell);
            parent_[neighborCell] = currCell;
            openDeque.push_back(neighborCell);
        }

        // take Snapshot AFTER adding neighbors
        if (record_search_snapshots_)
        {
            snapshot.frontier_ = extractNodes(openDeque);
            snapshot.explored_ = extractNodes(visitedSet);
            snapshots_.push_back(snapshot);
        }
    }

    // no path found
    metrics_.nodes_expanded = visitedSet.size();
}

void Agent::runDFS()
{
    snapshots_.clear();
    snapshot_index_ = 0;
    path_.clear();

    Snapshot snapshot;
    std::deque<Cell*> openDeque;
    std::unordered_set<Cell*> visitedSet;

    // start cell
    parent_[start_cell_] = nullptr;
    openDeque.push_back(start_cell_);
    visitedSet.insert(start_cell_);

    if (record_search_snapshots_)
    {
        snapshot.frontier_ = extractNodes(openDeque);
        snapshot.explored_ = extractNodes(visitedSet);
        snapshots_.push_back(snapshot);
    }

    // processing current cell in open set
    while (!openDeque.empty())
    {
        Cell* currCell = openDeque.back();
        openDeque.pop_back();

        // if current cell is goal, reconstruct and return path
        if (*currCell == *goal_cell_)
        {
            std::vector<Cell*> path;

            while (currCell != nullptr)
            {
                path.push_back(currCell);
                currCell = parent_.at(currCell);
            }
            // change from goal-to-start order to start-to-goal order
            std::reverse(path.begin(), path.end());
            path_ = path;
            metrics_.nodes_expanded = visitedSet.size() + 1;
            return;
        }

        // for each neighbor of current cell
        for (auto& n : getValidNeighbors(*currCell))
        {
            Cell* neighborCell = const_cast<Cell*>(&grid_.cells_[n.first][n.second]);

            // if neighbor in closedList (we have visited)
            if (visitedSet.find(neighborCell) != visitedSet.end())
            {
                continue;
            }

            // if we haven't visited the cell
            visitedSet.insert(neighborCell);
            parent_[neighborCell] = currCell;
            openDeque.push_back(neighborCell);
        }

        // take Snapshot AFTER adding neighbors
        if (record_search_snapshots_)
        {
            snapshot.frontier_ = extractNodes(openDeque);
            snapshot.explored_ = extractNodes(visitedSet);
            snapshots_.push_back(snapshot);
        }
    }

    // no path found
    metrics_.nodes_expanded = visitedSet.size();
}

sf::Color Agent::getColor() const
{
    return color_;
}

Cell * Agent::getStartCell() const
{
    return start_cell_;
}

Cell * Agent::getGoalCell() const
{
    return goal_cell_;
}

double Agent::heuristic(const Cell &currCell, const Cell &goalCell)
{
    // Manhattan distance
    double dx = std::abs(currCell.x_ - goalCell.x_);
    double dy = std::abs(currCell.y_ - goalCell.y_);
    return (dx + dy) * 1.001; // tiny nudge breaks ties
}

std::vector<std::pair<int, int>> Agent::getValidNeighbors(const Cell &currCell) const
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
        if ((pn.first >= 0 && pn.second >=0) && (pn.first < grid_.getRows() && pn.second < grid_.getCols()) && (grid_.cells_[pn.first][pn.second].getType() != CellType::obstacle))
        {
            valid_neighbors.push_back(pn);
        }
    }
    return valid_neighbors;
}

std::vector<Cell *> Agent::extractNodes(const std::set<Cell *, CompareCell> &set)
{
    return std::vector<Cell*>(set.begin(), set.end());
}

std::vector<Cell *> Agent::extractNodes(const std::unordered_set<Cell *> &unordered_set)
{
    return std::vector<Cell*>(unordered_set.begin(), unordered_set.end());
}

std::vector<Cell *> Agent::extractNodes(const std::deque<Cell*>& deque)
{
    return std::vector<Cell*>(deque.begin(), deque.end());
}

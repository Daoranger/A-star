// Copyright 2026, Hoang Nguyen
//
// grid.h
//
// Defines the Grid class, which represents the 2D grid consisted
// of a m x n cells, uses for interaction and visualization

#ifndef PATHFINDING_GRID_H
#define PATHFINDING_GRID_H

#include <vector>
#include <queue>
#include <functional>
#include <cmath>
#include "cell.h"
#include "../pathfinding/Snapshot.h"
#include <unordered_set>
#include <set>

struct CompareCell
{
    bool operator()(Cell* a, Cell* b) const
    {
        if (a->f_ != b->f_)
            return a->f_ < b->f_;  // lower f = higher priority

        return a < b;
    }
};

class Grid
{
public:

    Grid(std::size_t rows, std::size_t cols, float cellSize);
    void draw(sf::RenderWindow& window);
    std::size_t getRows() const;
    std::size_t getCols() const;
    float getCellSize() const;
    std::vector<Cell*> astar(std::vector<Snapshot>& snapshots, Snapshot& snapshot, std::size_t& nodesExpanded);
    double heuristic(const Cell& currCell, const Cell& goalCell);
    std::vector<std::pair<int, int>> getValidNeighbors(const Cell& currCell);
    void resetCells();
    std::vector<Cell*> extractNodes(const std::set<Cell*,CompareCell>& set);
    std::vector<Cell*> extractNodes(const std::unordered_set<Cell*>& unordered_set);

    Cell* start_cell_;
    Cell* goal_cell_;

private:
    std::size_t rows_;
    std::size_t cols_;
    float cell_size_;

public:
    std::vector<std::vector<Cell>> cells_;
};


#endif //PATHFINDING_GRID_H
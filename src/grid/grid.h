// Copyright 2026, Hoang Nguyen
//
// grid.h
//
// Defines the Grid class, which represents the 2D grid consisted
// of a m x n cells, uses for interaction and visualization

#ifndef PATHFINDING_GRID_H
#define PATHFINDING_GRID_H

#include "grid.h"

#include <cmath>
#include <set>
#include <unordered_set>
#include <vector>
#include <algorithm>

#include "../pathfinding/snapshot.h"
#include "cell.h"

// Return true if cell a have lower f value than cell b.
// Used by the open set as it acts like a priority queue,
// where cell is lower f have higher priority
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

    // Construct cells on grid and assign them with
    // their corresponding row and column on the grid
    Grid(std::size_t rows, std::size_t cols, float cellSize);

    // Use offset to draw the grid in the middle of the window.
    // Loop through each cell in the grid and draw at the correct
    // position
    void draw(sf::RenderWindow& window);

    // Get number of rows in the grid
    std::size_t getRows() const;

    // Get number of columns in the grid
    std::size_t getCols() const;

    // Get cell size
    float getCellSize() const;

    // A* algorithm.
    // Return the path represent as a list of cells. Take in snapshots for visualization animation.
    // Take in nodes expanded to display the sum of frontier + visisted noted
    std::vector<Cell*> astar(std::vector<Snapshot>& snapshots, Snapshot& snapshot, std::size_t& nodesExpanded);

    // Return valid neighbors of current cell
    std::vector<std::pair<int, int>> getValidNeighbors(const Cell& currCell);

    Cell* start_cell_;
    Cell* goal_cell_;

private:
    std::size_t rows_;
    std::size_t cols_;
    float cell_size_;

    // Return the distance between the current cell to goal cell
    double heuristic(const Cell& currCell, const Cell& goalCell);

    // Uses to extract nodes in open set for snapshots
    std::vector<Cell*> extractNodes(const std::set<Cell*,CompareCell>& set);

    // Uses to extract nodes in closed set for snapshots
    std::vector<Cell*> extractNodes(const std::unordered_set<Cell*>& unordered_set);

public:
    std::vector<std::vector<Cell>> cells_;
};


#endif //PATHFINDING_GRID_H
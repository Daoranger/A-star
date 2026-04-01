//
// Created by hoang on 2/13/2026.
//

#ifndef PATHFINDING_GRID_H
#define PATHFINDING_GRID_H

#include <vector>
#include <queue>
#include <functional>
#include <cmath>
#include "Cell.h"
#include "Snapshot.h"
#include <unordered_set>
#include <set>

struct CompareCell
{
    bool operator()(Cell* a, Cell* b) const
    {
        if (a->m_f != b->m_f)
            return a->m_f < b->m_f;  // lower f = higher priority

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

    Cell* m_startCell;
    Cell* m_goalCell;

private:
    std::size_t m_rows;
    std::size_t m_cols;
    float m_cellSize;


public:
    std::vector<std::vector<Cell>> m_cells;

};


#endif //PATHFINDING_GRID_H
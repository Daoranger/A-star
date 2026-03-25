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
    [[nodiscard]] std::size_t getRows() const;
    [[nodiscard]] std::size_t getCols() const;
    [[nodiscard]] float getCellSize() const;
    std::vector<Cell*> astar();
    double heuristic(const Cell& currCell, const Cell& goalCell);
    std::vector<std::pair<int, int>> getValidNeighbors(const Cell& currCell);
    void resetCells();

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
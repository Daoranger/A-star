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

class Grid
{
public:

    Grid(std::size_t rows, std::size_t cols, float cellSize);
    void draw(sf::RenderWindow& window);
    [[nodiscard]] std::size_t getRows() const;
    [[nodiscard]] std::size_t getCols() const;
    [[nodiscard]] float getCellSize() const;
    void astar(Cell& startCell, Cell& goalCell);
    double heuristic(const Cell& currCell, const Cell& goalCell);

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
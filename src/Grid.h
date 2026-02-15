//
// Created by hoang on 2/13/2026.
//

#ifndef PATHFINDING_GRID_H
#define PATHFINDING_GRID_H

#include <vector>
#include "Cell.h"


class Grid
{
public:
    Grid();
    void draw(sf::RenderWindow& window);
    std::size_t getRows();
    std::size_t getCols();
    void toggleCellBlocked(std::size_t row, std::size_t col);

private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<std::vector<Cell>> m_grid;

};


#endif //PATHFINDING_GRID_H
#ifndef PATHFINDING_CELL_H
#define PATHFINDING_CELL_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <iostream>

enum class CellType
{
    start,
    goal,
    obstacle,
    open,
};

class Cell
{
public:
    Cell();
    explicit Cell(float cellSize);
    void draw(sf::RenderWindow& window);
    void setSquarePosition(const sf::Vector2f& pos);
    void setCellType(CellType cellType);

private:
    sf::RectangleShape m_square;
    CellType m_cellType;

};

#endif //PATHFINDING_CELL_H
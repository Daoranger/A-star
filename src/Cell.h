#ifndef PATHFINDING_CELL_H
#define PATHFINDING_CELL_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <iostream>

enum class CellType
{
    start = 0,
    goal = 1,
    obstacle = 2,
    open = 3,
};

class Cell
{
public:

    Cell(float cellSize, int x, int y);
    void draw(sf::RenderWindow& window);
    void setSquarePosition(const sf::Vector2f& pos);
    void setCellType(CellType cellType);
    CellType getCellType() const;

    bool operator>(const Cell& other) const;  // overloaded operator > so it can be use in greater<> comparator
    friend std::ostream& operator<<(std::ostream& out, const Cell& cell);
    bool operator==(const Cell& other);

    int m_x;
    int m_y;

    double m_g;
    double m_h;
    double m_f;       // f = g + h
    Cell* m_parent;

private:
    sf::RectangleShape m_square;
    CellType m_cellType;

};

#endif //PATHFINDING_CELL_H
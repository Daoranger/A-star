#ifndef PATHFINDING_CELL_H
#define PATHFINDING_CELL_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Cell
{
public:
    Cell();
    explicit Cell(float cellSize);
    void draw(sf::RenderWindow& window);
    void setSquarePosition(const sf::Vector2f& pos);
    void setbBlocked(bool bBlocked);
    bool getbBlocked() const;

private:
    sf::RectangleShape m_square;
    bool m_bBlocked;

};


#endif //PATHFINDING_CELL_H
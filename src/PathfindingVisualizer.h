//
// Created by hoang on 2/13/2026.
//

#ifndef PATHFINDING_PATHFINDING_H
#define PATHFINDING_PATHFINDING_H

#include "Grid.h"
#include "SFML/Graphics/RenderWindow.hpp"


class PathfindingVisualizer
{

public:

    PathfindingVisualizer();
    void run();
    void processEvents();
    void update();
    void draw();
    void handleSelecting(const sf::Event::MouseMoved& mouseEvent);
    void handleDeslecting(const sf::Event::MouseMoved& mouseEvent);


private:
    sf::RenderWindow m_window;
    Grid m_grid;
    CellType m_currentCellType {CellType::start};
    bool m_bStartSelected;
    bool m_bGoalSelected;
    bool m_bisDragging;
    bool m_bSelecting;

};


#endif //PATHFINDING_PATHFINDING_H
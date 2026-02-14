//
// Created by hoang on 2/13/2026.
//

#ifndef PATHFINDING_PATHFINDING_H
#define PATHFINDING_PATHFINDING_H

#include "Cell.h"
#include "SFML/Graphics/RenderWindow.hpp"


class PathfindingVisualizer
{

public:

    PathfindingVisualizer();
    void run();
    void processEvents();
    void update();
    void draw();

private:

    sf::RenderWindow m_window;
    Cell m_cell;
};


#endif //PATHFINDING_PATHFINDING_H
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

private:
    sf::RenderWindow m_window;
    Grid m_grid;

    void handleMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseEvent);
};


#endif //PATHFINDING_PATHFINDING_H
//
// Created by hoang on 2/13/2026.
//

#ifndef PATHFINDING_PATHFINDING_H
#define PATHFINDING_PATHFINDING_H

#include "Grid.h"
#include "SFML/Graphics/RenderWindow.hpp"


class Game
{

public:

    Game();
    void run();
    void processEvents();
    void update();
    void draw();
    void handleDragSelecting(const sf::Event::MouseMoved& mouseEvent);
    void handleDragDeselecting(const sf::Event::MouseMoved& mouseEvent);
    void handleClickToggling(const sf::Event::MouseButtonPressed& mouseEvent, const bool bSelecting);
    void handleAStar();

private:
    sf::RenderWindow m_window;
    Grid m_grid;
    CellType m_currentCellType {CellType::start};
    bool m_bStartSelected;
    bool m_bGoalSelected;
    bool m_bisDragging;
    bool m_bSelecting;
    bool m_bPathGenerated;

};


#endif //PATHFINDING_PATHFINDING_H
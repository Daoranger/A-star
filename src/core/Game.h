//
// Created by hoang on 2/13/2026.
//

#ifndef PATHFINDING_PATHFINDING_H
#define PATHFINDING_PATHFINDING_H

#include "../grid/grid.h"
#include "../pathfinding/Snapshot.h"
#include "Metrics.h"
#include "SFML/Graphics/RenderWindow.hpp"


class Game
{

public:

    Game();
    void run();
    void processEvents();
    void update();
    void draw();
    void handleDragSelecting(const sf::Event::MouseMoved& mouseEvent, const sf::Vector2f& worldPos);
    void handleDragDeselecting(const sf::Event::MouseMoved& mouseEvent, const sf::Vector2f& worldPos);
    void handleClickToggling(const sf::Event::MouseButtonPressed& mouseEvent, const sf::Vector2f& worldPos, const bool bSelecting);
    void handleAStar(std::vector<Snapshot>& snapshots, Snapshot& snapshot);
    void clearSnapshots();

private:
    sf::RenderWindow m_window;
    sf::View m_view;
    Grid m_grid;
    CellType m_currentCellType {CellType::start};
    std::vector<Snapshot> m_snapshots;
    Snapshot m_snapshot;
    int m_snapshotIndex = 0;
    sf::Clock m_snapshotClock;
    float m_delay = 0.001;
    std::vector<Cell*> path;

    sf::Clock m_imguiClock;
    Metrics m_metrics;

    bool m_bStartSelected;
    bool m_bGoalSelected;
    bool m_bisDragging;
    bool m_bSelecting;
    bool m_bPathGenerated;
    bool m_bFinishedAnimation = false;

};


#endif //PATHFINDING_PATHFINDING_H
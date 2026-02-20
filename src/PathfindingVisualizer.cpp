//
// Created by hoang on 2/13/2026.
//

#include "PathfindingVisualizer.h"
#include <iostream>

PathfindingVisualizer::PathfindingVisualizer()
    : m_window(sf::VideoMode( { 1200, 700 } ), "A* Pathfinding")
    , m_grid(10, 50, 50)
    , m_bStartSelected(false)
    , m_bGoalSelected(false)
    , m_bisDragging(false)
    , m_bSelecting(true)
{
}

void PathfindingVisualizer::run()
{
    while (m_window.isOpen() )
    {
        processEvents();
        update();
        draw();
    }
}

void PathfindingVisualizer::processEvents()
{
    while ( const std::optional event = m_window.pollEvent() )
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        // on resized event
        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {
            // created a view from fixed visible area using FloatRect
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
            m_window.setView(sf::View(visibleArea));
        }

        // on mouse button pressed
        if (const auto* mouseButtonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            m_bisDragging = true;

        }

        if (const auto* keyPressedEvent = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num1)
            {
                if (!m_bSelecting)
                    m_bSelecting = true;
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num2)
            {
                if (m_bSelecting)
                    m_bSelecting = false;
            }
        }

        if (const auto* mouseButtonReleasedEvent = event->getIf<sf::Event::MouseButtonReleased>())
        {
            m_bisDragging = false;
        }

        if (const auto* mouseMovedEvent = event->getIf<sf::Event::MouseMoved>())
        {
            if (m_bisDragging && m_bSelecting)
            {
                handleSelecting(*mouseMovedEvent);
            }
            else if (m_bisDragging && !m_bSelecting)
            {
                handleDeslecting(*mouseMovedEvent);
            }

        }
    }
}

void PathfindingVisualizer::update()
{
    if (!m_bStartSelected)
    {
        m_currentCellType = CellType::start;
    }
    else if (!m_bGoalSelected)
    {
        m_currentCellType = CellType::goal;
    }
    else
    {
        m_currentCellType = CellType::obstacle;
    }
}

void PathfindingVisualizer::draw()
{
    m_window.clear(sf::Color::White);
    m_grid.draw(m_window);
    m_window.display();
}

void PathfindingVisualizer::handleSelecting(const sf::Event::MouseMoved& mouseEvent)
{
    std::cout << "(" << mouseEvent.position.x << ", " << mouseEvent.position.y << ")\n";
    const int row = mouseEvent.position.x / m_grid.getCellSize();
    const int col = mouseEvent.position.y / m_grid.getCellSize();

    // if clicking outside the grid
    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        std::cout << "Clicking outside the grid\n";
        return;
    }

    // switch case to handle toggling start, goal, and obstacle
    if (m_grid.m_cells[row][col].getCellType() == CellType::open || m_grid.m_cells[row][col].getCellType() == CellType::obstacle)
    {
        if (!m_bStartSelected)
        {
            m_bStartSelected = true;
        }
        else if (!m_bGoalSelected)
        {
            m_bGoalSelected = true;
        }
        // set the cell with this row and col in the m_grid with the current cell type
        m_grid.m_cells[row][col].setCellType(m_currentCellType);
    }
}

void PathfindingVisualizer::handleDeslecting(const sf::Event::MouseMoved &mouseEvent)
{
    std::cout << "(" << mouseEvent.position.x << ", " << mouseEvent.position.y << ")\n";
    const int row = mouseEvent.position.x / m_grid.getCellSize();
    const int col = mouseEvent.position.y / m_grid.getCellSize();

    // if clicking outside the grid
    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        std::cout << "Clicking outside the grid\n";
        return;
    }

    switch (m_grid.m_cells[row][col].getCellType())
    {
        case CellType::start:
        {
            m_bStartSelected = false;
        }
        case CellType::goal:
        {
            m_bGoalSelected = false;
        }
    }
    // switch case to handle toggling start, goal, and obstacle
    m_grid.m_cells[row][col].setCellType(CellType::open);
}

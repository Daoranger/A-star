//
// Created by hoang on 2/13/2026.
//

#include "PathfindingVisualizer.h"
#include <iostream>

PathfindingVisualizer::PathfindingVisualizer()
    : m_window{sf::VideoMode( { 1200, 700 } ), "SFML works!"}
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
            handleMouseButtonPressed(*mouseButtonPressedEvent);
        }
    }
}

void PathfindingVisualizer::update()
{
}

void PathfindingVisualizer::draw()
{
    m_window.clear();
    m_grid.draw(m_window);
    m_window.display();
}

void PathfindingVisualizer::handleMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseEvent)
{
    std::cout << "(" << mouseEvent.position.x << ", " << mouseEvent.position.y << ")\n";
    int row = mouseEvent.position.x / 50;
    int col = mouseEvent.position.y / 50;

    // if clicking outside the grid
    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        std::cout << "Clicking outside the grid\n";
        return;
    }

    m_grid.toggleCellBlocked(row, col);

}

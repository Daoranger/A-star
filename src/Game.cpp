//
// Created by hoang on 2/13/2026.
//

#include "Game.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode( { 1200, 700 } ), "A* Pathfinding")
    , m_grid(20, 20, 50)
    , m_bStartSelected(false)
    , m_bGoalSelected(false)
    , m_bisDragging(false)
    , m_bSelecting(true)
    , m_bPathGenerated(false)
{
}

void Game::run()
{
    while (m_window.isOpen() )
    {
        processEvents();
        update();
        draw();
    }
}

void Game::processEvents()
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
            m_view = sf::View(visibleArea);
            m_window.setView(m_view);
        }

        // on mouse button pressed
        if (const auto* mouseButtonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            handleClickToggling(*mouseButtonPressedEvent, m_bSelecting);
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
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num3)
            {
                if (!m_bPathGenerated)
                {
                    handleAStar();
                    m_bPathGenerated = true;
                }
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num4)
            {
                m_grid.resetCells();
                m_bPathGenerated = false;
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
                handleDragSelecting(*mouseMovedEvent);
            }
            else if (m_bisDragging && !m_bSelecting)
            {
                handleDragDeselecting(*mouseMovedEvent);
            }
        }

        if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if (mouseWheelScrolled->delta > 0)
            {
                std::cout << "Scrolling up\n";
                m_view.zoom(0.9f);
            }
            else if (mouseWheelScrolled->delta < 0)
            {
                std::cout << "Scrolling down\n";
                m_view.zoom(1.1f);
            }

            m_window.setView(m_view);
        }
    }
}

void Game::update()
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

void Game::draw()
{
    m_window.clear(sf::Color::White);
    m_grid.draw(m_window);
    m_window.display();
}

void Game::handleDragSelecting(const sf::Event::MouseMoved& mouseEvent)
{
    const int row = mouseEvent.position.x / m_grid.getCellSize();
    const int col = mouseEvent.position.y / m_grid.getCellSize();

    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        return;
    }

    switch (m_grid.m_cells[row][col].getCellType())
    {
        case CellType::open:
        case CellType::obstacle:
        {
            m_grid.m_cells[row][col].setCellType(m_currentCellType);
            if (!m_bStartSelected)
            {
                m_grid.m_startCell = &m_grid.m_cells[row][col];
                m_bStartSelected = true;
            }
            else if (!m_bGoalSelected)
            {
                m_grid.m_goalCell = &m_grid.m_cells[row][col];
                m_bGoalSelected = true;
            }
            break;
        }
        case CellType::start:
        {
            break;
        }
        case CellType::goal:
        {
            break;
        }
    }

}

void Game::handleDragDeselecting(const sf::Event::MouseMoved &mouseEvent)
{
    const int row = mouseEvent.position.x / m_grid.getCellSize();
    const int col = mouseEvent.position.y / m_grid.getCellSize();

    // if clicking outside the grid
    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        return;
    }

    switch (m_grid.m_cells[row][col].getCellType())
    {

        case CellType::start:
        {
            m_grid.m_cells[row][col].setCellType(CellType::open);
            m_grid.m_startCell = nullptr;
            if (m_bStartSelected)

                m_bStartSelected = false;
            break;
        }
        case CellType::goal:
        {
            m_grid.m_cells[row][col].setCellType(CellType::open);
            m_grid.m_goalCell = nullptr;
            if (m_bGoalSelected)
                m_bGoalSelected = false;
            break;
        }
        case CellType::obstacle:
        {
            m_grid.m_cells[row][col].setCellType(CellType::open);
            break;
        }
        case CellType::open:
        {
            break;
        }
    }
}

void Game::handleClickToggling(const sf::Event::MouseButtonPressed &mouseEvent, const bool isSelecting)
{
    const int row = mouseEvent.position.x / m_grid.getCellSize();
    const int col = mouseEvent.position.y / m_grid.getCellSize();

    // if clicking outside the grid
    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        return;
    }

    // switch case to handle toggling start, goal, and obstacle
    if (isSelecting)
    {
        switch (m_grid.m_cells[row][col].getCellType())
        {
            case CellType::open:
            case CellType::obstacle:
            {
                m_grid.m_cells[row][col].setCellType(m_currentCellType);
                if (!m_bStartSelected)
                {
                    m_grid.m_startCell = &m_grid.m_cells[row][col];
                    m_bStartSelected = true;
                }
                else if (!m_bGoalSelected)
                {
                    m_grid.m_goalCell = &m_grid.m_cells[row][col];
                    m_bGoalSelected = true;
                }
                break;
            }
            case CellType::start:
            {
                break;
            }
            case CellType::goal:
            {
                break;
            }
        }
    }
    else
    {
        switch (m_grid.m_cells[row][col].getCellType())
        {

            case CellType::start:
            {
                m_grid.m_cells[row][col].setCellType(CellType::open);
                m_grid.m_startCell = nullptr;
                if (m_bStartSelected)

                    m_bStartSelected = false;
                break;
            }
            case CellType::goal:
            {
                m_grid.m_cells[row][col].setCellType(CellType::open);
                m_grid.m_goalCell = nullptr;
                if (m_bGoalSelected)
                    m_bGoalSelected = false;
                break;
            }
            case CellType::obstacle:
            {
                m_grid.m_cells[row][col].setCellType(CellType::open);
                break;
            }
            case CellType::open:
            {
                break;
            }
        }
    }
}

void Game::handleAStar()
{
    if (m_bGoalSelected && m_bStartSelected)
    {
        std::vector<Cell*> path = m_grid.astar();

        std::cout << "path size: " << path.size() << "\n";

        if (path.size() > 0)
        {
            // exclude the start and goal cell for now
            for (int i = 1; i < path.size() - 1; ++i)
            {
                path[i]->setCellType(CellType::path);
                //std::cout << path[i]->m_x << " " << path[i]->m_y << "\n";
            }
        }
        else
        {
            std::cout << "No path exist\n";
        }
    }
    else
    {
        std::cout << "Start cell or Goal cell is not selected\n";
    }
}
//
// Created by hoang on 2/13/2026.
//

#include "../core//Game.h"
#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"

Game::Game()
    : m_window(sf::VideoMode( { 1200, 700 } ), "A* Pathfinding")
    , m_view(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(m_window.getPosition().x, m_window.getPosition().y)))
    , m_grid(100, 100, 100)
    , m_bStartSelected(false)
    , m_bGoalSelected(false)
    , m_bisDragging(false)
    , m_bSelecting(true)
    , m_bPathGenerated(false)
{
    m_window.setView(m_view);
    ImGui::SFML::Init(m_window);
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
        ImGui::SFML::ProcessEvent(m_window, *event);

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

        // Mouse Button Pressed Event
        if (const auto* mouseButtonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            sf::Vector2i pixelPos = mouseButtonPressedEvent->position;
            sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos, m_view);

            sf::Vector2f gridOffset(
                m_window.getSize().x / 2.f - m_grid.getCellSize() * m_grid.getCols() / 2.f,
                m_window.getSize().y / 2.f - m_grid.getCellSize() * m_grid.getRows() / 2.f
            );
            sf::Vector2f localPos = worldPos - gridOffset;

            handleClickToggling(*mouseButtonPressedEvent, localPos, m_bSelecting);
            m_bisDragging = true;

        }

        // Keyboard Keys Pressed Events
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
                    handleAStar(m_snapshots, m_snapshot);
                    m_bPathGenerated = true;
                }
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num4)
            {
                clearSnapshots();
                m_bPathGenerated = false;
            }
        }

        // Mouse Button Released Event
        if (const auto* mouseButtonReleasedEvent = event->getIf<sf::Event::MouseButtonReleased>())
        {
            m_bisDragging = false;
        }

        // Mouse Drag Event
        if (const auto* mouseMovedEvent = event->getIf<sf::Event::MouseMoved>())
        {
            sf::Vector2i pixelPos = mouseMovedEvent->position;
            sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos, m_view);

            sf::Vector2f gridOffset(
                m_window.getSize().x / 2.f - m_grid.getCellSize() * m_grid.getCols() / 2.f,
                m_window.getSize().y / 2.f - m_grid.getCellSize() * m_grid.getRows() / 2.f
            );
            sf::Vector2f localPos = worldPos - gridOffset;

            if (m_bisDragging && m_bSelecting)
            {
                handleDragSelecting(*mouseMovedEvent, localPos);
            }
            else if (m_bisDragging && !m_bSelecting)
            {
                handleDragDeselecting(*mouseMovedEvent, localPos);
            }
        }

        // Mouse Wheel Scrolled Event
        if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if (mouseWheelScrolled->delta > 0)
            {
                m_view.zoom(0.9f);
            }
            else if (mouseWheelScrolled->delta < 0)
            {
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

    if (m_bPathGenerated)
    {
        if (m_snapshotClock.getElapsedTime().asSeconds() > m_delay)
        {
            if (m_snapshotIndex < m_snapshots.size() - 1)
                m_snapshotIndex++;
            else
                m_bFinishedAnimation = true;
            m_snapshotClock.restart();
        }

        if (!m_bFinishedAnimation)
        {
            m_snapshots[m_snapshotIndex].prepareSnapshot();
        }
    }

    if (path.size() > 0 && m_bFinishedAnimation)
    {
        for (int i = 1; i < path.size() - 1; ++i)
        {
            path[i]->setType(CellType::path);
        }
    }

    ImGui::SFML::Update(m_window, m_imguiClock.restart());

    ImGui::SetNextWindowSize(ImVec2(400, 200)); // change ImGUI size
    ImGui::Begin("Metrics");                    // start ImGUI window, with title "Metrics"
    ImGui::SetWindowFontScale(2.0f);            // scale up font of ImGUI by 2
    ImGui::Text("Path Found: %s", m_metrics.m_pathFound ? "Yes" : "No");
    ImGui::Text("Path Length: %zu", m_metrics.m_pathSize);
    ImGui::Text("Nodes Expanded: %zu", m_metrics.m_nodesExpanded);
    ImGui::Text("Search Time: %.2f ms", m_metrics.m_searchTime);
    ImGui::End();

}

void Game::draw()
{
    m_window.clear(sf::Color::White);
    m_grid.draw(m_window);
    ImGui::SFML::Render(m_window);
    m_window.display();
}

void Game::handleDragSelecting(const sf::Event::MouseMoved& mouseEvent, const sf::Vector2f& worldPos)
{
    const int row = worldPos.x / m_grid.getCellSize();
    const int col = worldPos.y / m_grid.getCellSize();

    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        return;
    }

    switch (m_grid.cells_[row][col].getType())
    {
        case CellType::open:
        case CellType::obstacle:
        {
            m_grid.cells_[row][col].setType(m_currentCellType);
            if (!m_bStartSelected)
            {
                m_grid.start_cell_ = &m_grid.cells_[row][col];
                m_bStartSelected = true;
            }
            else if (!m_bGoalSelected)
            {
                m_grid.goal_cell_ = &m_grid.cells_[row][col];
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

void Game::handleDragDeselecting(const sf::Event::MouseMoved &mouseEvent, const sf::Vector2f& worldPos)
{
    const int row = worldPos.x / m_grid.getCellSize();
    const int col = worldPos.y / m_grid.getCellSize();

    // if clicking outside the grid
    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        return;
    }

    switch (m_grid.cells_[row][col].getType())
    {

        case CellType::start:
        {
            m_grid.cells_[row][col].setType(CellType::open);
            m_grid.start_cell_ = nullptr;
            if (m_bStartSelected)

                m_bStartSelected = false;
            break;
        }
        case CellType::goal:
        {
            m_grid.cells_[row][col].setType(CellType::open);
            m_grid.goal_cell_ = nullptr;
            if (m_bGoalSelected)
                m_bGoalSelected = false;
            break;
        }
        case CellType::obstacle:
        {
            m_grid.cells_[row][col].setType(CellType::open);
            break;
        }
        case CellType::open:
        {
            break;
        }
    }
}

void Game::handleClickToggling(const sf::Event::MouseButtonPressed &mouseEvent, const sf::Vector2f& worldPos, const bool isSelecting)
{
    const int row = worldPos.x / m_grid.getCellSize();
    const int col = worldPos.y / m_grid.getCellSize();

    // if clicking outside the grid
    if (row >= m_grid.getRows() || col >= m_grid.getCols())
    {
        return;
    }

    // switch case to handle toggling start, goal, and obstacle
    if (isSelecting)
    {
        switch (m_grid.cells_[row][col].getType())
        {
            case CellType::open:
            case CellType::obstacle:
            {
                m_grid.cells_[row][col].setType(m_currentCellType);
                if (!m_bStartSelected)
                {
                    m_grid.start_cell_ = &m_grid.cells_[row][col];
                    m_bStartSelected = true;
                }
                else if (!m_bGoalSelected)
                {
                    m_grid.goal_cell_ = &m_grid.cells_[row][col];
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
        switch (m_grid.cells_[row][col].getType())
        {

            case CellType::start:
            {
                m_grid.cells_[row][col].setType(CellType::open);
                m_grid.start_cell_ = nullptr;
                if (m_bStartSelected)

                    m_bStartSelected = false;
                break;
            }
            case CellType::goal:
            {
                m_grid.cells_[row][col].setType(CellType::open);
                m_grid.goal_cell_ = nullptr;
                if (m_bGoalSelected)
                    m_bGoalSelected = false;
                break;
            }
            case CellType::obstacle:
            {
                m_grid.cells_[row][col].setType(CellType::open);
                break;
            }
            case CellType::open:
            {
                break;
            }
        }
    }
}

void Game::handleAStar(std::vector<Snapshot>& snapshots, Snapshot& snapshot)
{
    std::size_t nodesExpanded {};

    if (m_bGoalSelected && m_bStartSelected)
    {
        auto start = std::chrono::high_resolution_clock::now();
        path = m_grid.astar(snapshots, snapshot, nodesExpanded);
        auto end = std::chrono::high_resolution_clock::now();

        m_metrics.m_pathFound = !path.empty();
        m_metrics.m_pathSize = path.size();
        m_metrics.m_nodesExpanded = nodesExpanded;
        m_metrics.m_searchTime = std::chrono::duration<double, std::milli>(end - start).count();
    }
    else
    {
        std::cout << "Start cell or Goal cell is not selected\n";
    }
}

void Game::clearSnapshots()
{
    m_snapshots.clear();
    m_snapshot.clearSnapshot();
}

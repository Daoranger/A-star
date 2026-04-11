#include "../core/game.h"

#include <iostream>
#include <chrono>

#include "imgui.h"
#include "imgui-SFML.h"

Game::Game()
    : window_(sf::VideoMode( { 1200, 700 } ), "A* Pathfinding")
    , view_(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(window_.getPosition().x, window_.getPosition().y)))
    , grid_(6, 6, 100)
{
    window_.setView(view_);
    ImGui::SFML::Init(window_);
}

void Game::run()
{
    while (window_.isOpen() )
    {
        processEvents();
        update();
        draw();
    }
}

void Game::processEvents()
{
    while ( const std::optional event = window_.pollEvent() )
    {
        ImGui::SFML::ProcessEvent(window_, *event);

        if (event->is<sf::Event::Closed>())
            window_.close();

        // on resized event
        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {
            // created a view from fixed visible area using FloatRect
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
            view_ = sf::View(visibleArea);
            window_.setView(view_);
        }

        // Mouse Button Pressed Event
        if (const auto* mouseButtonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            sf::Vector2i pixelPos = mouseButtonPressedEvent->position;
            sf::Vector2f worldPos = window_.mapPixelToCoords(pixelPos, view_);

            sf::Vector2f gridOffset(
                window_.getSize().x / 2.f - grid_.getCellSize() * grid_.getCols() / 2.f,
                window_.getSize().y / 2.f - grid_.getCellSize() * grid_.getRows() / 2.f
            );
            sf::Vector2f localPos = worldPos - gridOffset;

            onMouseClick(*mouseButtonPressedEvent, localPos, is_selecting_);
            is_dragging_ = true;

        }

        // Keyboard Keys Pressed Events
        if (const auto* keyPressedEvent = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num1)
            {
                if (!is_selecting_)
                    is_selecting_ = true;
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num2)
            {
                if (is_selecting_)
                    is_selecting_ = false;
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num3)
            {
                if (!path_generated_)
                {
                    runAStar();
                    path_generated_ = true;
                }
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num4)
            {
                path_generated_ = false;
            }
        }

        // Mouse Button Released Event
        if (const auto* mouseButtonReleasedEvent = event->getIf<sf::Event::MouseButtonReleased>())
        {
            is_dragging_ = false;
        }

        // Mouse Drag Event
        if (const auto* mouseMovedEvent = event->getIf<sf::Event::MouseMoved>())
        {
            sf::Vector2i pixelPos = mouseMovedEvent->position;
            sf::Vector2f worldPos = window_.mapPixelToCoords(pixelPos, view_);

            sf::Vector2f gridOffset(
                window_.getSize().x / 2.f - grid_.getCellSize() * grid_.getCols() / 2.f,
                window_.getSize().y / 2.f - grid_.getCellSize() * grid_.getRows() / 2.f
            );
            sf::Vector2f localPos = worldPos - gridOffset;

            if (is_dragging_ && is_selecting_)
            {
                onDragSelect(*mouseMovedEvent, localPos);
            }
            else if (is_dragging_ && !is_selecting_)
            {
                onDragDeselect(*mouseMovedEvent, localPos);
            }
        }

        // Mouse Wheel Scrolled Event
        if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if (mouseWheelScrolled->delta > 0)
            {
                view_.zoom(0.9f);
            }
            else if (mouseWheelScrolled->delta < 0)
            {
                view_.zoom(1.1f);
            }

            window_.setView(view_);
        }
    }
}

void Game::update()
{
    if (!start_selected_)
    {
        current_cell_type_ = CellType::start;
    }
    else if (!goal_selected_)
    {
        current_cell_type_ = CellType::goal;
    }
    else
    {
        current_cell_type_ = CellType::obstacle;
    }

    if (path_generated_)
    {
        if (snapshot_clock_.getElapsedTime().asSeconds() > delay_)
        {
            if (snapshot_index_ < snapshots_.size() - 1)
                snapshot_index_++;
            else
                animation_finished_ = true;
            snapshot_clock_.restart();
        }

        if (!animation_finished_)
        {
            snapshots_[snapshot_index_].prepareSnapshot();
        }
    }

    if (path_.size() > 0 && animation_finished_)
    {
        for (int i = 1; i < path_.size() - 1; ++i)
        {
            path_[i]->setType(CellType::path);
        }
    }

    ImGui::SFML::Update(window_, imgui_clock_.restart());

    ImGui::SetNextWindowSize(ImVec2(400, 200)); // change ImGUI size
    ImGui::Begin("Metrics");                    // start ImGUI window, with title "Metrics"
    ImGui::SetWindowFontScale(2.0f);            // scale up font of ImGUI by 2
    ImGui::Text("Path Found: %s", metrics_.path_found ? "Yes" : "No");
    ImGui::Text("Path Length: %zu", metrics_.path_size);
    ImGui::Text("Nodes Expanded: %zu", metrics_.nodes_expanded);
    ImGui::Text("Search Time: %.2f ms", metrics_.search_time);
    ImGui::End();

}

void Game::draw()
{
    window_.clear(sf::Color::White);
    grid_.draw(window_);
    ImGui::SFML::Render(window_);
    window_.display();
}

void Game::onDragSelect(const sf::Event::MouseMoved& mouseEvent, const sf::Vector2f& worldPos)
{
    const int row = worldPos.x / grid_.getCellSize();
    const int col = worldPos.y / grid_.getCellSize();

    if (row >= grid_.getRows() || col >= grid_.getCols())
    {
        return;
    }

    switch (grid_.cells_[row][col].getType())
    {
        case CellType::open:
        case CellType::obstacle:
        {
            grid_.cells_[row][col].setType(current_cell_type_);
            if (!start_selected_)
            {
                grid_.start_cell_ = &grid_.cells_[row][col];
                start_selected_ = true;
            }
            else if (!goal_selected_)
            {
                grid_.goal_cell_ = &grid_.cells_[row][col];
                goal_selected_ = true;
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

void Game::onDragDeselect(const sf::Event::MouseMoved &mouseEvent, const sf::Vector2f& worldPos)
{
    const int row = worldPos.x / grid_.getCellSize();
    const int col = worldPos.y / grid_.getCellSize();

    // if clicking outside the grid
    if (row >= grid_.getRows() || col >= grid_.getCols())
    {
        return;
    }

    switch (grid_.cells_[row][col].getType())
    {

        case CellType::start:
        {
            grid_.cells_[row][col].setType(CellType::open);
            grid_.start_cell_ = nullptr;
            if (start_selected_)

                start_selected_ = false;
            break;
        }
        case CellType::goal:
        {
            grid_.cells_[row][col].setType(CellType::open);
            grid_.goal_cell_ = nullptr;
            if (goal_selected_)
                goal_selected_ = false;
            break;
        }
        case CellType::obstacle:
        {
            grid_.cells_[row][col].setType(CellType::open);
            break;
        }
        case CellType::open:
        {
            break;
        }
    }
}

void Game::onMouseClick(const sf::Event::MouseButtonPressed &mouseEvent, const sf::Vector2f& worldPos, const bool isSelecting)
{
    const int row = worldPos.x / grid_.getCellSize();
    const int col = worldPos.y / grid_.getCellSize();

    // if clicking outside the grid
    if (row >= grid_.getRows() || col >= grid_.getCols())
    {
        return;
    }

    // switch case to handle toggling start, goal, and obstacle
    if (isSelecting)
    {
        switch (grid_.cells_[row][col].getType())
        {
            case CellType::open:
            case CellType::obstacle:
            {
                grid_.cells_[row][col].setType(current_cell_type_);
                if (!start_selected_)
                {
                    grid_.start_cell_ = &grid_.cells_[row][col];
                    start_selected_ = true;
                }
                else if (!goal_selected_)
                {
                    grid_.goal_cell_ = &grid_.cells_[row][col];
                    goal_selected_ = true;
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
        switch (grid_.cells_[row][col].getType())
        {

            case CellType::start:
            {
                grid_.cells_[row][col].setType(CellType::open);
                grid_.start_cell_ = nullptr;
                if (start_selected_)

                    start_selected_ = false;
                break;
            }
            case CellType::goal:
            {
                grid_.cells_[row][col].setType(CellType::open);
                grid_.goal_cell_ = nullptr;
                if (goal_selected_)
                    goal_selected_ = false;
                break;
            }
            case CellType::obstacle:
            {
                grid_.cells_[row][col].setType(CellType::open);
                break;
            }
            case CellType::open:
            {
                break;
            }
        }
    }
}

void Game::runAStar()
{
    std::size_t nodesExpanded {};

    if (goal_selected_ && start_selected_)
    {
        auto start = std::chrono::high_resolution_clock::now();
        path_ = grid_.astar(snapshots_, snapshot_, nodesExpanded);
        auto end = std::chrono::high_resolution_clock::now();

        metrics_.path_found = !path_.empty();
        metrics_.path_size = path_.size();
        metrics_.nodes_expanded = nodesExpanded;
        metrics_.search_time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    else
    {
        std::cout << "Start cell or Goal cell is not selected\n";
    }
}

sf::Vector2f Game::getGridOffset() const
{
}

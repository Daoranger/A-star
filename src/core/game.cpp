#include "../core/game.h"

#include <iostream>
#include <chrono>

#include "imgui.h"
#include "imgui-SFML.h"

Game::Game()
    : window_(sf::VideoMode( { 1200, 700 } ), "A* Pathfinding")
    , view_(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(window_.getPosition().x, window_.getPosition().y)))
    , grid_(50, 50, 50)
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
            is_dragging_ = true;

            sf::Vector2i pixelPos = mouseButtonPressedEvent->position;
            sf::Vector2f worldPos = window_.mapPixelToCoords(pixelPos, view_);

            sf::Vector2f gridOffset = getGridOffset();
            sf::Vector2f localPos = worldPos - gridOffset;

            onMouseClick(*mouseButtonPressedEvent, localPos);
        }

        // Keyboard Keys Pressed Events
        if (const auto* keyPressedEvent = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num1)
            {
                input_mode_ = InputMode::kSelecting;
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num2)
            {
                input_mode_ = InputMode::kDeselecting;
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num3)
            {
                if (app_state_ == AppState::kIdle)
                {
                    if ((game_mode_ == GameMode::kSingleAgent && placement_state_ == PlacementState::kPlacingObstacles) || game_mode_ == GameMode::kMultiAgentSequential)
                    {
                        runAStar();
                        app_state_ = AppState::kAnimating;
                    }
                }
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Num4)
            {
                reset();
            }
            else if (keyPressedEvent->scancode == sf::Keyboard::Scancode::Tab)
            {
                if (game_mode_ == GameMode::kSingleAgent)
                {
                    game_mode_ = GameMode::kMultiAgentSequential;
                }
                else
                {
                    game_mode_ = GameMode::kSingleAgent;
                }
                reset();
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

            sf::Vector2f gridOffset = getGridOffset();
            sf::Vector2f localPos = worldPos - gridOffset;

            if (is_dragging_)
            {
                onDrag(*mouseMovedEvent, localPos);
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
    if (app_state_ == AppState::kAnimating && !agents_.empty())
    {
        if (snapshot_clock_.getElapsedTime().asSeconds() > delay_)
        {
            bool all_done = true;
            for (auto& agent : agents_)
            {
                if (agent->snapshot_index_ < agent->snapshots_.size() - 1)
                {
                    agent->snapshot_index_++;
                    all_done = false;
                }
            }
            if (all_done)
                app_state_ = AppState::kDone;
            snapshot_clock_.restart();
        }
    }

    ImGui::SFML::Update(window_, imgui_clock_.restart());
    ImGui::SetNextWindowSize(ImVec2(330, 170));
    ImGui::Begin("Metrics");
    ImGui::SetWindowFontScale(2.0f);
    for (auto& agent : agents_)
    {
        ImGui::Text("Path Found: %s", agent->metrics_.path_found ? "Yes" : "No");
        ImGui::Text("Path Length: %zu", agent->metrics_.path_size);
        ImGui::Text("Nodes Expanded: %zu", agent->metrics_.nodes_expanded);
        ImGui::Text("Search Time: %.2f ms", agent->metrics_.search_time);
        ImGui::Separator();
    }
    ImGui::End();
}

void Game::draw()
{
    window_.clear(sf::Color::White);
    grid_.draw(window_);
    drawAgents();
    ImGui::SFML::Render(window_);
    window_.display();
}

void Game::drawAgents()
{
    for (auto& agent : agents_)
    {
        drawAgent(*agent);
    }
}

void Game::drawAgent(Agent& agent)
{
    sf::Color color = agent.getColor();
    sf::Vector2f offset = getGridOffset();
    sf::RectangleShape overlay(sf::Vector2f(grid_.getCellSize(), grid_.getCellSize()));

    const Cell* startCell = agent.getStartCell();
    const Cell* goalCell = agent.getGoalCell();

    if (!startCell || !goalCell) return;

    color.a = 255;
    overlay.setFillColor(color);
    overlay.setPosition(sf::Vector2f(startCell->x_ * grid_.getCellSize(), startCell->y_ * grid_.getCellSize()) + offset);
    window_.draw(overlay);

    color.a = 128;
    overlay.setFillColor(color);
    overlay.setPosition(sf::Vector2f(goalCell->x_ * grid_.getCellSize(), goalCell->y_ * grid_.getCellSize()) + offset);
    window_.draw(overlay);

    switch (app_state_)
    {

        case AppState::kAnimating:
        {
            if (agent.snapshots_.empty()) return;
            Snapshot& snapshot = agent.snapshots_[agent.snapshot_index_];

            color.a = 128;
            overlay.setFillColor(color);
            for (Cell* cell : snapshot.frontier_)
            {
                overlay.setPosition(sf::Vector2f(cell->x_ * grid_.getCellSize(), cell->y_ * grid_.getCellSize()) + offset);
                window_.draw(overlay);
            }

            color.a = 64;
            overlay.setFillColor(color);
            for (Cell* cell : snapshot.explored_)
            {
                overlay.setPosition(sf::Vector2f(cell->x_ * grid_.getCellSize(), cell->y_ * grid_.getCellSize()) + offset);
                window_.draw(overlay);
            }

            break;
        }
        case AppState::kDone:
        {
            if (agent.snapshots_.empty()) return;
            color.a = 150;
            overlay.setFillColor(color);
            for (Cell* cell : agent.path_)
            {
                overlay.setPosition(sf::Vector2f(cell->x_ * grid_.getCellSize(), cell->y_ * grid_.getCellSize()) + offset);
                window_.draw(overlay);
            }
            break;
        }
    }
}


void Game::onDrag(const sf::Event::MouseMoved &mouseEvent, const sf::Vector2f &worldPos)
{
    if (app_state_ != AppState::kIdle) return;

    const int row = worldPos.x / grid_.getCellSize();
    const int col = worldPos.y / grid_.getCellSize();

    if (row >= grid_.getRows() || row < 0 || col >= grid_.getCols() || col < 0)
    {
        return;
    }

    switch (input_mode_)
    {
        case InputMode::kSelecting:
        {
            selectCell(row, col);
            break;
        }
        case InputMode::kDeselecting:
        {
            deselectCell(row, col);
            break;
        }
    }
}

void Game::onMouseClick(const sf::Event::MouseButtonPressed &mouseEvent, const sf::Vector2f &worldPos)
{
    if (app_state_ != AppState::kIdle) return;

    const int row = worldPos.x / grid_.getCellSize();
    const int col = worldPos.y / grid_.getCellSize();

    if (row >= grid_.getRows() || row < 0 || col >= grid_.getCols() || col < 0)
    {
        return;
    }

    switch (input_mode_)
    {
        case InputMode::kSelecting:
        {
            selectCell(row, col);
            break;
        }
        case InputMode::kDeselecting:
        {
            deselectCell(row, col);
            break;
        }
    }
}

void Game::runAStar()
{
    auto start = std::chrono::high_resolution_clock::now();

    if (game_mode_ == GameMode::kSingleAgent)
    {
        if (placement_state_ != PlacementState::kPlacingObstacles) return;
        agents_.clear();
        agents_.push_back(std::make_unique<Agent>(start_cell_, goal_cell_, grid_, sf::Color::Blue));
    }

    for (auto& agent : agents_)
    {
        agent->runAStar();
    }

    auto end = std::chrono::high_resolution_clock::now();

    for (auto& agent : agents_)
    {
        agent->metrics_.search_time = std::chrono::duration<double, std::milli>(end - start).count();
        agent->metrics_.path_found = !agent->path_.empty();
        agent->metrics_.path_size = agent->path_.size();
    }
}

void Game::initAgents()
{
    agents_.push_back(std::make_unique<Agent>(&grid_.cells_[0][0], &grid_.cells_[49][49], grid_, sf::Color::Red));
    agents_.push_back(std::make_unique<Agent>(&grid_.cells_[0][49], &grid_.cells_[49][0], grid_, sf::Color::Blue));
}

sf::Vector2f Game::getGridOffset() const
{
    float x = window_.getSize().x / 2.f - grid_.getCellSize() * grid_.getRows() / 2.f;
    float y = window_.getSize().y / 2.f - grid_.getCellSize() * grid_.getCols() / 2.f;
    return sf::Vector2f(x, y);
}

void Game::selectCell(int row, int col)
{
    if (game_mode_ != GameMode::kSingleAgent)
    {
        if (grid_.cells_[row][col].getType() == CellType::open)
        {
            grid_.cells_[row][col].setType(CellType::obstacle);
        }
        return;
    }

    switch (placement_state_)
    {
        case PlacementState::kNeedsStart:
        {
            if (grid_.cells_[row][col].getType() == CellType::goal)
            {
                break;
            }

            if (start_cell_)
                start_cell_->setType(CellType::open);

            grid_.cells_[row][col].setType(CellType::start);
            start_cell_ = &grid_.cells_[row][col];
            placement_state_ = PlacementState::kNeedsGoal;

            break;
        }
        case PlacementState::kNeedsGoal:
        {
            if (grid_.cells_[row][col].getType() == CellType::start)
            {
                break;
            }

            if (goal_cell_)
                goal_cell_->setType(CellType::open);

            grid_.cells_[row][col].setType(CellType::goal);
            goal_cell_ = &grid_.cells_[row][col];
            placement_state_ = PlacementState::kPlacingObstacles;
            break;
        }
        case PlacementState::kPlacingObstacles:
        {
            if (grid_.cells_[row][col].getType() != CellType::start &&
                    grid_.cells_[row][col].getType() != CellType::goal)
            {
                grid_.cells_[row][col].setType(CellType::obstacle);
            }
            break;
        }
    }
}

void Game::deselectCell(int row, int col)
{
    switch (grid_.cells_[row][col].getType())
    {
        case CellType::start:
        {
            grid_.cells_[row][col].setType(CellType::open);
            start_cell_ = nullptr;
            placement_state_ = PlacementState::kNeedsStart;
            break;
        }
        case CellType::goal:
        {
            grid_.cells_[row][col].setType(CellType::open);
            goal_cell_ = nullptr;
            placement_state_ = start_cell_ ? PlacementState::kNeedsGoal : PlacementState::kNeedsStart;
            break;
        }
        case CellType::obstacle:
        {
            grid_.cells_[row][col].setType(CellType::open);
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::reset()
{
    for (int i = 0; i < grid_.getRows(); ++i)
    {
        for (int j = 0; j < grid_.getCols(); ++j)
        {
            grid_.cells_[i][j].reset();
        }
    }

    agents_.clear();

    if (game_mode_ == GameMode::kMultiAgentSequential || game_mode_ == GameMode::kMultiAgentThreaded)
    {
        initAgents();
    }

    start_cell_ = nullptr;
    goal_cell_ = nullptr;
    placement_state_ = PlacementState::kNeedsStart;
    app_state_ = AppState::kIdle;
}

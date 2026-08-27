//
// Created by hoang on 5/8/2026.
//

#include "Game.h"

#include <iostream>

#include "SFML/Graphics/CircleShape.hpp"

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(3072, 1920)), "Steering Behaviors")
    , vehicle1(sf::Vector2f(1400, 900))
    , vehicle2(sf::Vector2f(1500, 950))
    , vehicle3(sf::Vector2f(1450, 1000))
    , patrolPath_(std::vector<sf::Vector2f> {{1200.f, 700.f},
      {1900.f, 700.f},
      {1900.f, 1200.f},
      {1200.f, 1200.f}}, true)
{

    vehicles_ = {&vehicle1, &vehicle2, &vehicle3};
    vehicle2.steeringBehaviors.setPath(&patrolPath_);

    spawnObstacles(3);

    walls.push_back(std::make_unique<Wall>(
    sf::Vector2f(1200.0f, 700.0f),
    sf::Vector2f(1500.0f, 600.0f)
));

    walls.push_back(std::make_unique<Wall>(
        sf::Vector2f(1500.0f, 600.0f),
        sf::Vector2f(1800.0f, 750.0f)
    ));

    walls.push_back(std::make_unique<Wall>(
        sf::Vector2f(1800.0f, 750.0f),
        sf::Vector2f(1600.0f, 1050.0f)
    ));

    walls.push_back(std::make_unique<Wall>(
        sf::Vector2f(1600.0f, 1050.0f),
        sf::Vector2f(1200.0f, 700.0f)
    ));
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while ( const std::optional event = window.pollEvent() )
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
            window.setView(sf::View(visibleArea));
        }
    }
}

void Game::update()
{

    // for mouse as target
    float dt = clock.restart().asSeconds();
    sf::Vector2f target = sf::Vector2f(sf::Mouse::getPosition(window));

    sf::Vector2f flockForce1 = vehicle1.steeringBehaviors.flock(vehicles_);
    vehicle1.update(dt, flockForce1, window.getSize());

    sf::Vector2f flockForce2 = vehicle2.steeringBehaviors.flock(vehicles_);
    vehicle2.update(dt, flockForce2, window.getSize());

    sf::Vector2f flockForce3 = vehicle3.steeringBehaviors.flock(vehicles_);
    vehicle3.update(dt, flockForce3, window.getSize());
}

void Game::render()
{
    window.clear(sf::Color::Black);
    vehicle1.render(window);
    vehicle2.render(window);
    vehicle3.render(window);

    // draw patrol path waypoints
    // for (const auto& point : patrolPath_.waypoints_)
    // {
    //     sf::CircleShape marker(8.f);
    //     marker.setFillColor(sf::Color::Red);
    //     marker.setOrigin(sf::Vector2f(8.f, 8.f)); // center the circle on the point
    //     marker.setPosition(point);
    //     window.draw(marker);
    // }

    // drawing obst
    // for (const auto& obstacle : obstacles)
    // {
    //     obstacle->render(window);
    // }


    // for (auto& wall : walls)
    // {
    //     wall->render(window);
    // }

    window.display();
}

void Game::spawnObstacles(int amount)
{
    for (int i = 0; i < 1; i++)
    {
        auto obstacle = std::make_unique<Obstacle>(randomInRange(50.0f, 200.0f));
        float posX = randomInRange(0, 1920);
        float posY = randomInRange(0, 1080);
        obstacle->setPosition(sf::Vector2f(posX, posY));
        obstacles.push_back(std::move(obstacle));
    }
}

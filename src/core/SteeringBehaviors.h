//
// Created by hoang on 6/3/2026.
//

#ifndef STEERINGBEHAVIORS_STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_STEERINGBEHAVIORS_H

#include "Obstacle.h"
#include "Path.h"
#include "Wall.h"
#include "SFML/System/Vector2.hpp"

class Vehicle;

class SteeringBehaviors
{
public:
    SteeringBehaviors(Vehicle& vehicle);

    sf::Vector2f seek(sf::Vector2f target) const;
    sf::Vector2f flee(sf::Vector2f target) const;
    sf::Vector2f arrive(sf::Vector2f target) const;
    sf::Vector2f pursuit(const Vehicle& evader) const;
    sf::Vector2f evade(const Vehicle& pursuer) const;
    sf::Vector2f wander();
    sf::Vector2f obstacleAvoidance(const std::vector<std::unique_ptr<Obstacle>>& obstacles);
    sf::Vector2f wallAvoidance(const std::vector<std::unique_ptr<Wall>>& walls);
    sf::Vector2f interpose(const Vehicle& agentA, const Vehicle& agentB);
    sf::Vector2f hide(const Vehicle& target, const std::vector<std::unique_ptr<Obstacle>>& obstacles);
    sf::Vector2f followPath();
    sf::Vector2f offsetPursuit(const Vehicle& leader, sf::Vector2f offset);
    sf::Vector2f separation(const std::vector<Vehicle*>& vehicles);
    sf::Vector2f alignment(const std::vector<Vehicle*>& vehicles);
    sf::Vector2f cohesion(const std::vector<Vehicle*>& vehicles);


    float wanderRadius = 30.0f;
    float wanderDistance = 100.0f;
    float wanderJitter = 1.0f;
    sf::Vector2f wanderTarget;
    float middleFeelerLength = 300.0f;
    float sideFeelerLength = 200.0f;
    double waypointSeekDistSq = 100.0;
    float neighborRadius = 300.0f;

    // only store endpoints
    std::vector<sf::Vector2f> feelers;

    void setPath(Path* path) { path_ = path; }


private:
    Vehicle& vehicle_;
    Path* path_;

    sf::Vector2f pointToWorldSpace(sf::Vector2f targetLocal);
    sf::Vector2f pointToWorldSpace(sf::Vector2f targetLocal, const Vehicle& referenceVehicle);
    sf::Vector2f pointToLocalSpace(sf::Vector2f targetWorld);
    sf::Vector2f vectorToWorldSpace(sf::Vector2f vecLocal);
    bool lineIntersection2D(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d, float& dist, sf::Vector2f& point);
    void tagObstaclesInRange(const std::vector<std::unique_ptr<Obstacle>>& obstacles, float range);
    void createFeelers();
    sf::Vector2f rotateVector(const sf::Vector2f& vector, float angleDegrees);
    sf::Vector2f getHidingPositions(const sf::Vector2f& obstaclePos, const float obstacleRadius, const sf::Vector2f& targetPos);
    void tagNeighborVehicleInRange(const std::vector<Vehicle*>& vehicles, float range);
};

#endif //STEERINGBEHAVIORS_STEERINGBEHAVIORS_H

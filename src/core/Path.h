//
// Created by hoang on 8/25/2026.
//

#ifndef STEERINGBEHAVIORS_PATH_H
#define STEERINGBEHAVIORS_PATH_H
#include <vector>

#include "SFML/System/Vector2.hpp"


class Path
{
public:
    Path() = default;
    Path(std::vector<sf::Vector2f> waypoints, bool looped);
    sf::Vector2f currentWaypoint() const;
    void setNextWaypoint();
    bool finished() const;
    bool empty() const;

    std::vector<sf::Vector2f> waypoints_;

private:
    std::size_t currentIndex_ = 0;
    bool isLooped_ = false;
};



#endif //STEERINGBEHAVIORS_PATH_H

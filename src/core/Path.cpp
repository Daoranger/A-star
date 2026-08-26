//
// Created by hoang on 8/25/2026.
//

#include "Path.h"

Path::Path(std::vector<sf::Vector2f> waypoints, bool looped)
    : waypoints_(std::move(waypoints))
    , isLooped_(looped)
{
}

sf::Vector2f Path::currentWaypoint() const
{
    return waypoints_[currentIndex_];
}

void Path::setNextWaypoint()
{
    if (currentIndex_ + 1 < waypoints_.size())
    {
        ++currentIndex_;
    }
    else if (isLooped_)
    {
        currentIndex_ = 0;
    }
}

bool Path::finished() const
{
    return waypoints_.empty() || (!isLooped_ && currentIndex_ == waypoints_.size() - 1);
}

bool Path::empty() const
{
    return waypoints_.empty();
}

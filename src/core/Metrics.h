//
// Created by hoang on 4/1/2026.
//

#ifndef PATHFINDING_METRICS_H
#define PATHFINDING_METRICS_H
#include <cstddef>

struct Metrics
{
    std::size_t m_pathSize {};
    std::size_t m_nodesExpanded {};
    double m_searchTime {};
    bool m_pathFound {false};
};

#endif //PATHFINDING_METRICS_H
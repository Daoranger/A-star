//
// Created by hoang on 3/30/2026.
//

#ifndef PATHFINDING_SNAPSHOT_H
#define PATHFINDING_SNAPSHOT_H
#include <vector>
#include "Cell.h"


class Snapshot
{
public:
    Snapshot();
    void clearSnapshot();
    std::vector<Cell*> m_openVector;
    std::vector<Cell*> m_closedVector;
};


#endif //PATHFINDING_SNAPSHOT_H
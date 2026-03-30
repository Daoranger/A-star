//
// Created by hoang on 3/30/2026.
//

#include "Snapshot.h"

Snapshot::Snapshot()
    : m_openVector{}
    , m_closedVector{}
{
}

void Snapshot::prepareSnapshot()
{

    for (auto& cell : m_openVector)
    {
        if ((cell->getCellType() == CellType::start) || (cell->getCellType() == CellType::goal))
        {
            continue;
        }
        cell->setCellType(CellType::frontier);
    }

    for (auto& cell : m_closedVector)
    {
        if ((cell->getCellType() == CellType::start) || (cell->getCellType() == CellType::goal))
        {
            continue;
        }
        cell->setCellType(CellType::explored);
    }
}

void Snapshot::clearSnapshot()
{
    m_openVector.clear();
    m_closedVector.clear();
}

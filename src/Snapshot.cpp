//
// Created by hoang on 3/30/2026.
//

#include "Snapshot.h"

Snapshot::Snapshot()
    : m_openVector{}
    , m_closedVector{}
{
}

void Snapshot::clearSnapshot()
{
    m_openVector.clear();
    m_closedVector.clear();
}

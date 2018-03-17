#include "StaticCollisionConstraint.h"


csb::StaticCollisionConstraint::~StaticCollisionConstraint() = default;

void csb::StaticCollisionConstraint::setHashTableSize(const size_t&_newSize)
{
  m_hashTableSize = _newSize;
}

void csb::StaticCollisionConstraint::setCellSize(const float _newSize)
{
  m_cellSize = _newSize;
}


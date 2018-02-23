#include "CSBconstraint.h"

CSBconstraint::~CSBconstraint() = default;

void DistanceConstraint::project(std::vector<glm::vec3> &_positions)
{
  auto restSqr = m_distance * m_distance;
  auto& p1 = _positions[m_p1];
  auto& p2 = _positions[m_p2];
  auto delta = p2 - p1;
  delta *= restSqr / (delta * delta + restSqr) - 0.5f;
  p1 -= delta;
  p2 += delta;
}



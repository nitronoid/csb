#include "CSBconstraint.h"
#include "gtx/fast_square_root.hpp"

CSBconstraint::~CSBconstraint() = default;

void DistanceConstraint::project(std::vector<glm::vec3> &_positions)
{
  auto& p1 = _positions[m_p1];
  auto& p2 = _positions[m_p2];
  auto delta = p2 - p1;
  auto deltaLen = glm::fastLength(delta);
  auto diff = (deltaLen - m_distance) / deltaLen;
  delta *= 0.5f * diff;

  p1 += delta;
  p2 -= delta;

}



#include "DistanceConstraint.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/fast_square_root.hpp"
#undef GLM_ENABLE_EXPERIMENTAL

void csb::DistanceConstraint::project(std::vector<Particle> &_positions)
{
  auto& p1 = _positions[m_p1];
  auto& p2 = _positions[m_p2];
  auto delta = *p2.m_pos - *p1.m_pos;
  auto deltaLen = glm::fastLength(delta);
  auto diff = (deltaLen - m_rest) / (deltaLen * (p1.m_invMass + p2.m_invMass));
  delta *= diff;

  *p1.m_pos += (delta * p1.m_invMass);
  *p2.m_pos -= (delta * p2.m_invMass);
}

csb::PositionConstraint* csb::DistanceConstraint::clone() const
{
  return new DistanceConstraint(*this);
}

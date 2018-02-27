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

void BendingConstraint::project(std::vector<glm::vec3> &_positions)
{
  auto p1 = _positions[m_p1];
  auto p2 = _positions[m_p2];
  auto p3 = _positions[m_p3];

  constexpr float third = 1.0f / 3.0f;
  auto centre = third * (p1 + p2 + p3);
  auto dirCentre = p3 - centre;

  volatile float x = centre.x;
  volatile float y = centre.x;
  volatile float z = centre.x;
  volatile float x1 = p3.x;
  volatile float y2 = p3.x;
  volatile float z3 = p3.x;
  volatile auto distCentre = glm::fastLength(dirCentre);

  float diff = 1.0f - (m_distance / distCentre);
  auto force = dirCentre;
  force *= diff;

  _positions[m_p1] += (0.0f * 2.f * force);
  _positions[m_p2] += (0.0f * 2.f * force);
  _positions[m_p3] += (0.0f * -4.f * force);
}

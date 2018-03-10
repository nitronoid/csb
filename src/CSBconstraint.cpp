#include "CSBconstraint.h"
#include "gtx/fast_square_root.hpp"

CSBconstraint::~CSBconstraint() = default;

void DistanceConstraint::project(std::vector<CSBpoint> &_positions)
{
  auto& p1 = _positions[m_p1];
  auto& p2 = _positions[m_p2];
  auto delta = p2.m_pos - p1.m_pos;
  auto deltaLen = glm::fastLength(delta);
  auto diff = (deltaLen - m_rest) / (deltaLen * (p1.m_invMass + p2.m_invMass));
  delta *= diff;

  p1.m_pos += (delta * p1.m_invMass);
  p2.m_pos -= (delta * p2.m_invMass);
}

void BendingConstraint::project(std::vector<CSBpoint> &_positions)
{
  auto& p1 = _positions[m_p[0]];
  auto& p2 = _positions[m_p[1]];
  auto& p3 = _positions[m_p[2]];

  static constexpr float third = 1.0f / 3.0f;
  auto centre = third * (p1.m_pos + p2.m_pos + p3.m_pos);
  auto dirCentre = p3.m_pos - centre;

  auto distCentre = glm::fastLength(dirCentre);

  float sub = m_rest / distCentre;
  float diff = 1.0f;
  if (std::isfinite(sub))
    diff -= sub;
  auto force = dirCentre * diff;

  static constexpr auto k = 0.05f;
  p1.m_pos += (k * m_w[0] * 2.f * force);
  p2.m_pos += (k * m_w[1] * 2.f * force);
  p3.m_pos += (k * m_w[2] * -4.f * force);
}

void PinConstraint::project(std::vector<CSBpoint> &_positions)
{
  _positions[m_p].m_pos = m_pin;
}

void SelfCollisionConstraint::project(std::vector<CSBpoint> &_positions)
{
  auto& T0 = _positions[m_t[0]].m_pos;
  auto& T1 = _positions[m_t[1]].m_pos;
  auto& T2 = _positions[m_t[2]].m_pos;
  auto& P  = _positions[m_p ].m_pos;

  const auto delta = P - m_intersectionP;


  P  = m_intersectionP;

  T0 += ( delta);
  T1 += ( delta);
  T2 += ( delta);
}

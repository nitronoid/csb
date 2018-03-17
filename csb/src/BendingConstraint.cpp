#include "BendingConstraint.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/fast_square_root.hpp"
#undef GLM_ENABLE_EXPERIMENTAL


void csb::BendingConstraint::project(std::vector<Particle> &io_particles)
{
  auto& p1 = io_particles[m_p[0]];
  auto& p2 = io_particles[m_p[1]];
  auto& p3 = io_particles[m_p[2]];

  static constexpr float third = 1.0f / 3.0f;
  auto centre = third * (*p1.m_pos + *p2.m_pos + *p3.m_pos);
  glm::vec3 dirCentre = *p3.m_pos - centre;

  auto distCentre = glm::fastLength(dirCentre);

  float sub = m_rest / distCentre;
  float diff = 1.0f;
  if (std::isfinite(sub))
    diff -= sub;
  auto force = dirCentre * diff;

  static constexpr auto k = 0.05f;
  *p1.m_pos += (k * m_w[0] * 2.f * force);
  *p2.m_pos += (k * m_w[1] * 2.f * force);
  *p3.m_pos += (k * m_w[2] * -4.f * force);
}

csb::PositionConstraint* csb::BendingConstraint::clone() const
{
  return new BendingConstraint(*this);
}

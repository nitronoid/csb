#include "BendingConstraint.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/fast_square_root.hpp"
#undef GLM_ENABLE_EXPERIMENTAL

csb::BendingConstraint::BendingConstraint(
    const size_t _p1,
    const size_t _p2,
    const size_t _p3,
    const float _rest,
    const float _stiffness,
    const std::vector<Particle>&_particles
    ) :
  m_p({{_p1, _p2, _p3}}),
  m_rest(_rest),
  m_stiffness(_stiffness)
{
  auto W = _particles[_p1].m_invMass + _particles[_p2].m_invMass + 2.f * _particles[_p3].m_invMass;
  for (unsigned int i = 0; i < m_w.size(); ++i)
    m_w[i] = _particles[m_p[i]].m_invMass / W;
}

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

  *p1.m_pos += (m_stiffness * m_w[0] * 2.f * force);
  *p2.m_pos += (m_stiffness * m_w[1] * 2.f * force);
  *p3.m_pos += (m_stiffness * m_w[2] * -4.f * force);
}

csb::PositionConstraint* csb::BendingConstraint::clone() const
{
  return new BendingConstraint(*this);
}

size_t csb::BendingConstraint::getParticleIndex(const unsigned short _index) const noexcept
{
  return m_p[_index];
}

float csb::BendingConstraint::getParticleWeight(const unsigned short _index) const noexcept
{
  return m_w[_index];
}

float csb::BendingConstraint::getRest() const noexcept
{
  return m_rest;
}

float csb::BendingConstraint::getStiffness() const noexcept
{
  return m_stiffness;
}

void csb::BendingConstraint::setParticleIndex(const unsigned short _index, const size_t &_p)
{
  m_p[_index] = _p;
}

void csb::BendingConstraint::setParticleWeight(const unsigned short _index, const float _invMass)
{
  m_w[_index] = _invMass;
}

void csb::BendingConstraint::setRest(const float _rest)
{
  m_rest = _rest;
}

void csb::BendingConstraint::setStiffness(const float _stiffness)
{
  m_stiffness = _stiffness;
}

#include "DistanceConstraint.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/fast_square_root.hpp"
#undef GLM_ENABLE_EXPERIMENTAL

void csb::DistanceConstraint::project(std::vector<Particle> &io_particles)
{
  auto& p1 = io_particles[m_p1];
  auto& p2 = io_particles[m_p2];
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

float  csb::DistanceConstraint::getRest() const noexcept
{
  return m_rest;
}

size_t csb::DistanceConstraint::getParticleIndex1() const noexcept
{
  return m_p1;
}

size_t csb::DistanceConstraint::getParticleIndex2() const noexcept
{
  return m_p2;
}

void csb::DistanceConstraint::setRest(const float _rest)
{
  m_rest = _rest;
}

void csb::DistanceConstraint::setParticleIndex1(const size_t &_p1)
{
  m_p1 = _p1;
}

void csb::DistanceConstraint::setParticleIndex2(const size_t &_p2)
{
  m_p2 = _p2;
}


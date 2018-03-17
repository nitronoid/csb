#include "PinConstraint.h"


void csb::PinConstraint::project(std::vector<Particle> &io_particles)
{
  *io_particles[m_p].m_pos = m_pin;
}

csb::PositionConstraint* csb::PinConstraint::clone() const
{
  return new PinConstraint(*this);
}

size_t csb::PinConstraint::getParticleIndex() const noexcept
{
  return m_p;
}

glm::vec3 csb::PinConstraint::getPinPosition() const noexcept
{
  return m_pin;
}

void csb::PinConstraint::setParticleIndex(const size_t &_p)
{
  m_p = _p;
}

void csb::PinConstraint::setPinPosition(const glm::vec3 &_pin)
{
  m_pin = _pin;
}

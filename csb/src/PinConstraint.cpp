#include "PinConstraint.h"


void csb::PinConstraint::project(std::vector<Particle> &_particles)
{
  *_particles[m_p].m_pos = m_pin;
}

csb::PositionConstraint* csb::PinConstraint::clone() const
{
  return new PinConstraint(*this);
}

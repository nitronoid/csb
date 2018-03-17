#include "PinConstraint.h"


void csb::PinConstraint::project(std::vector<Particle> &io_particles)
{
  *io_particles[m_p].m_pos = m_pin;
}

csb::PositionConstraint* csb::PinConstraint::clone() const
{
  return new PinConstraint(*this);
}

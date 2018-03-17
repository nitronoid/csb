#ifndef PINCONSTRAINT_H
#define PINCONSTRAINT_H

#include "PositionConstraint.h"

namespace csb
{

class PinConstraint : public PositionConstraint
{
public:
  PinConstraint() = default;
  PinConstraint(const size_t &_p, const glm::vec3 &_pin) :
    m_pin(_pin),
    m_p(_p)
  {}
  PinConstraint(const PinConstraint&) = default;
  PinConstraint& operator=(const PinConstraint&) = default;
  PinConstraint(PinConstraint&&) = default;
  PinConstraint& operator=(PinConstraint&&) = default;
  virtual ~PinConstraint() override = default;
  virtual void project(std::vector<Particle> &io_particles) override;
  virtual PositionConstraint* clone() const override;

  size_t getParticleIndex() const noexcept;
  glm::vec3 getPinPosition() const noexcept;

  void setParticleIndex(const size_t &_p);
  void setPinPosition(const glm::vec3 &_pin);

private:
  glm::vec3 m_pin;
  size_t m_p;
};

}

#endif // PINCONSTRAINT_H

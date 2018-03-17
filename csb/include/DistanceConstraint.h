#ifndef DISTANCECONSTRAINT_H
#define DISTANCECONSTRAINT_H

#include "PositionConstraint.h"

namespace csb
{

class DistanceConstraint : public PositionConstraint
{
public:
  DistanceConstraint() = default;
  DistanceConstraint(const size_t _p1, const size_t _p2, float _rest) :
    m_rest(_rest),
    m_p1(_p1),
    m_p2(_p2)
  {}
  DistanceConstraint(const DistanceConstraint&) = default;
  DistanceConstraint& operator=(const DistanceConstraint&) = default;
  DistanceConstraint(DistanceConstraint&&) = default;
  DistanceConstraint& operator=(DistanceConstraint&&) = default;
  virtual ~DistanceConstraint() override = default;
  virtual void project(std::vector<Particle> &io_particles) override;
  virtual PositionConstraint* clone() const override;

  float  getRest() const noexcept;
  size_t getParticleIndex1() const noexcept;
  size_t getParticleIndex2() const noexcept;

  void setRest(const float _rest);
  void setParticleIndex1(const size_t &_p1);
  void setParticleIndex2(const size_t &_p2);

private:
  float m_rest = 0.f;
  size_t m_p1 = 0;
  size_t m_p2 = 0;
};

}

#endif // DISTANCECONSTRAINT_H

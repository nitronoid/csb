#ifndef DISTANCECONSTRAINT_H
#define DISTANCECONSTRAINT_H

#include "PositionConstraint.h"

namespace csb
{

class DistanceConstraint : public PositionConstraint
{
public:
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
  virtual void project(std::vector<Particle> &_positions) override;
  virtual PositionConstraint* clone() const override;

private:
  float m_rest;
  size_t m_p1;
  size_t m_p2;
};

}

#endif // DISTANCECONSTRAINT_H

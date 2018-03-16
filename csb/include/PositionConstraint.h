#ifndef CSBCONSTRAINT_H
#define CSBCONSTRAINT_H

#include <vector>
#include "Particle.h"

namespace csb
{

class PositionConstraint
{
public:
  PositionConstraint() = default;
  PositionConstraint(const PositionConstraint&) = default;
  PositionConstraint& operator=(const PositionConstraint&) = default;
  PositionConstraint(PositionConstraint&&) = default;
  PositionConstraint& operator=(PositionConstraint&&) = default;
  virtual ~PositionConstraint();
  virtual void project(std::vector<Particle> &_positions) = 0;
  virtual PositionConstraint* clone() const = 0;
};

}

#endif // CSBCONSTRAINT_H

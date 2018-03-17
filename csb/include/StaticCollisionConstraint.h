#ifndef STATICCOLLISIONCONSTRAINT_H
#define STATICCOLLISIONCONSTRAINT_H

#include <vector>
#include <QOpenGLFunctions>
#include "Particle.h"

namespace csb
{

class StaticCollisionConstraint
{
public:
  StaticCollisionConstraint() = default;
  StaticCollisionConstraint(const StaticCollisionConstraint&) = default;
  StaticCollisionConstraint& operator=(const StaticCollisionConstraint&) = default;
  StaticCollisionConstraint(StaticCollisionConstraint&&) = default;
  StaticCollisionConstraint& operator=(StaticCollisionConstraint&&) = default;
  virtual ~StaticCollisionConstraint();

  virtual void project(std::vector<Particle> &_particles, const std::vector<std::vector<std::pair<GLushort, GLushort>>>&_spatialHash) = 0;
  virtual StaticCollisionConstraint* clone() const = 0;
};

}

#endif // STATICCOLLISIONCONSTRAINT_H

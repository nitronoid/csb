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
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint(const StaticCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint& operator=(const StaticCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint(StaticCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint& operator=(StaticCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~StaticCollisionConstraint();
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  /// @param _spatialHash is a reference to the solver's hash table which can be used to make the collision,
  /// test more efficient.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(std::vector<Particle> &io_particles, const std::vector<std::vector<std::pair<GLushort, GLushort>>>&_spatialHash) = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual StaticCollisionConstraint* clone() const = 0;
};

}

#endif // STATICCOLLISIONCONSTRAINT_H

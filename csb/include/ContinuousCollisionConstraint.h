#ifndef CONTINUOUSCOLLISIONCONSTRAINT_H
#define CONTINUOUSCOLLISIONCONSTRAINT_H

#include <vector>
#include <QOpenGLFunctions>
#include "SimulatedMesh.h"
#include "SpatialHash.h"

namespace csb
{

class ContinuousCollisionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  ContinuousCollisionConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  ContinuousCollisionConstraint(const ContinuousCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  ContinuousCollisionConstraint& operator=(const ContinuousCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  ContinuousCollisionConstraint(ContinuousCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  ContinuousCollisionConstraint& operator=(ContinuousCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~ContinuousCollisionConstraint();
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  /// @param _spatialHash is a reference to the solver's hash table which can be used to make the collision,
  /// test more efficient.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(
      const size_t &_meshIndex,
      std::vector<std::shared_ptr<SimulatedMesh>> &_meshes,
      const SpatialHash::SpatialHashTable &_spatialHash
      ) = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual ContinuousCollisionConstraint* clone() const = 0;

protected:
  //-----------------------------------------------------------------------------------------------------
  /// @brief This gives derived classes access to a meshes particles.
  //-----------------------------------------------------------------------------------------------------
  std::vector<csb::Particle>& getParticles(SimulatedMesh&_mesh) const noexcept;
};

}


#endif // CONTINUOUSCOLLISIONCONSTRAINT_H

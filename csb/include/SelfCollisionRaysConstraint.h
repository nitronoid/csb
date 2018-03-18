#ifndef SELFCOLLISIONCONSTRAINTRAYS_H
#define SELFCOLLISIONCONSTRAINTRAYS_H


#include "ContinuousCollisionConstraint.h"

namespace csb
{

class SelfCollisionRaysConstraint : public ContinuousCollisionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionRaysConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionRaysConstraint(const SelfCollisionRaysConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionRaysConstraint& operator=(const SelfCollisionRaysConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionRaysConstraint(SelfCollisionRaysConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionRaysConstraint& operator=(SelfCollisionRaysConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~SelfCollisionRaysConstraint() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  /// @param _spatialHash is a reference to the solver's hash table which can be used to make the collision,
  /// test more efficient.
  //-----------------------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve cloth on cloth collisions by calculating line, triangle intersections.
  /// The intersecting points will have their velocities reversed, and their positions reverted to the,
  /// previous ones to simulate a bounce.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(
      const size_t &_meshIndex,
      std::vector<std::shared_ptr<SimulatedMesh>> &_meshes,
      const SpatialHash::SpatialHashTable &_spatialHash
      ) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual ContinuousCollisionConstraint* clone() const override;

};

}

#endif // SELFCOLLISIONCONSTRAINTRAYS_H

#ifndef SELFCOLLISIONSPHERESCONSTRAINT_H
#define SELFCOLLISIONSPHERESCONSTRAINT_H

#include "ContinuousCollisionConstraint.h"

namespace csb
{

class SelfCollisionSpheresConstraint : public ContinuousCollisionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionSpheresConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor.
  /// @param _diameter is size of the spheres that should be placed around all particles to prevent,
  ///  intersection.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionSpheresConstraint(const float _diameter) :
    m_sphereDiameter(_diameter)
  {}
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionSpheresConstraint(const SelfCollisionSpheresConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionSpheresConstraint& operator=(const SelfCollisionSpheresConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionSpheresConstraint(SelfCollisionSpheresConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SelfCollisionSpheresConstraint& operator=(SelfCollisionSpheresConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~SelfCollisionSpheresConstraint() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  /// @param _spatialHash is a reference to the solver's hash table which can be used to make the collision,
  /// test more efficient.
  //-----------------------------------------------------------------------------------------------------

  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve cloth on cloth collisions by representing particles as connected spheres,
  /// and checking whether these spheres intersect. Particles that are too close are projected away from,
  /// eachother.
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
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the sphere diameter for each particle.
  /// @param _diameter is the new diameter of the collision spheres.
  //-----------------------------------------------------------------------------------------------------
  void setSphereDiameter(const float _radius);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the sphere diameter for each particle.
  /// @return The size of the collision spheres.
  //-----------------------------------------------------------------------------------------------------
  float getSphereDiameter() const noexcept;

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief The size of the collision spheres.
  //-----------------------------------------------------------------------------------------------------
  float m_sphereDiameter = 1.f;
};

}

#endif // SELFCOLLISIONSPHERESCONSTRAINT_H

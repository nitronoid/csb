#ifndef SPHERECOLLISIONCONSTRAINT_H
#define SPHERECOLLISIONCONSTRAINT_H

#include "StaticCollisionConstraint.h"


namespace csb
{

class SphereCollisionConstraint : public StaticCollisionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  SphereCollisionConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  SphereCollisionConstraint(const SphereCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SphereCollisionConstraint& operator=(const SphereCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  SphereCollisionConstraint(SphereCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SphereCollisionConstraint& operator=(SphereCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~SphereCollisionConstraint() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor.
  /// @param _centre is the origin of the sphere.
  /// @param _radius is the radius of the sphere and difines it's size.
  /// @param _cellSize is the cell size that is used for spatial hashing.
  /// @param _hashTableSize is the hash table size used by the spatial hash.
  //-----------------------------------------------------------------------------------------------------
  SphereCollisionConstraint(const glm::vec3 &_centre, const float _radius, const float _cellSize, const size_t& _hashTableSize);
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects particles that enter the sphere away from the centre and onto the surface.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  /// @param _spatialHash is a reference to the solver's hash table which can be used to make the collision,
  /// test more efficient.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(std::vector<Particle> &io_particles, const std::vector<std::vector<std::pair<GLushort, GLushort>>>& _spatialHash) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual StaticCollisionConstraint* clone() const override;

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is stores the cells in the spatial hash that are within the spheres bounding box.
  //-----------------------------------------------------------------------------------------------------
  std::vector<size_t> m_cells;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The origin of the sphere.
  //-----------------------------------------------------------------------------------------------------
  glm::vec3 m_centre {0.f};
  //-----------------------------------------------------------------------------------------------------
  /// @brief The radius of the sphere.
  //-----------------------------------------------------------------------------------------------------
  float m_radius = 0.f;
};

}

#endif // SPHERECOLLISIONCONSTRAINT_H

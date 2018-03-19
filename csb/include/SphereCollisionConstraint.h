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
  //-----------------------------------------------------------------------------------------------------
  SphereCollisionConstraint(const glm::vec3 &_centre, const float _radius);
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects particles that enter the sphere away from the centre and onto the surface.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  /// @param _spatialHash is a reference to the solver's hash table which can be used to make the collision,
  /// test more efficient.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(std::vector<Particle> &io_particles, const SpatialHash::SpatialHashTable &_spatialHash) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual StaticCollisionConstraint* clone() const override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This init function will be called post setting the cell and hash table sizes, inside of the,
  /// solver, so any calculations involving them for spatial hashing should take place here.
  //-----------------------------------------------------------------------------------------------------
  virtual void init() override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This updates the list of cells that are within the spheres bounding box.
  //-----------------------------------------------------------------------------------------------------
  void updateBoundingBoxCells();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Allows read only access to the list of cells within the spheres bounding box. You should,
  /// call updateBoundingBoxCells prior to this, if the sphere's centre or radius is changing.
  /// @return a constant reference to the list of cells withing the spheres bounding box.
  //-----------------------------------------------------------------------------------------------------
  const std::vector<size_t> cells() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a copy of the sphere's centre.
  /// @return a 3D co-ordinate representing the centre of this sphere constraint.
  //-----------------------------------------------------------------------------------------------------
  glm::vec3 getCentre() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a copy of the sphere's radius.
  /// @return the radius of this sphere constraint.
  //-----------------------------------------------------------------------------------------------------
  float getRadius() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the centre of the sphere constraint. Update bounding box cells should be called after,
  /// this.
  /// @param _centre is the new centre for the sphere constraint
  //-----------------------------------------------------------------------------------------------------
  void setCentre(const glm::vec3 &_centre);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the radius of the sphere constraint. Update bounding box cells should be called after,
  /// this.
  /// @param _radius is the new radius for the sphere constraint
  //-----------------------------------------------------------------------------------------------------
  void setRadius(const float _radius);

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

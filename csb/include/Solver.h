#ifndef CSBSOLVER_H
#define CSBSOLVER_H

#include <QOpenGLFunctions>
#include <memory>
#include "SimulatedMesh.h"
#include "StaticCollisionConstraint.h"

namespace csb
{

class Solver
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  Solver();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Copy constructor.
  //-----------------------------------------------------------------------------------------------------
  Solver(const Solver&_rhs);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Solver& operator=(const Solver&_rhs);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  Solver(Solver&&);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Solver& operator=(Solver&&);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  ~Solver();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Adds the given mesh to our simulation.
  /// @param io_mesh is the mesh to add.
  //-----------------------------------------------------------------------------------------------------
  void addTriangleMesh(const std::shared_ptr<SimulatedMesh> &io_mesh);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Progresses the simulation forwards by the given timestep.
  /// @param _time is the timestep to advance by
  //-----------------------------------------------------------------------------------------------------
  void step(const float _time);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Uses the internal timer to advance the simulation by a fixed timestep.
  //-----------------------------------------------------------------------------------------------------
  void update();

  void addStaticCollision(StaticCollisionConstraint* _newConstraint);

  void addForce(const glm::vec3 &_force);

  glm::vec3 getTotalForce() const noexcept;

  void setTotalForce(const glm::vec3 &_force);

  void setDamping(const float _damping);

  float getDamping() const noexcept;


private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Iterates over and stores particle indices in the hash table using the hashParticle function.
  /// @param _meshIndex is the index of the referenced mesh, who's particles we should hash.
  //-----------------------------------------------------------------------------------------------------
  void hashVerts(const size_t& _meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Iterates over the triangles in a mesh and stores a list of vertices contained within the,
  /// cells that enter the triangles bounding box. The list is stored inside of the triangle-vert hash,
  /// table.
  /// @param _meshIndex is the index of the referenced mesh, who's triangles we should hash.
  //-----------------------------------------------------------------------------------------------------
  void hashTris(const size_t &_meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Uses the adjacency list within a mesh to return a list of particle indices that neighbour,
  /// the given particle. These indices are local the the mesh.
  /// @param io_meshRef is the referenced mesh, particles we should query.
  /// @param _particle is the index of the particle, who's neighbours we will retrieve.
  /// @return a list of particle indices, local to io_meshRef, which are neighbours to _vert.
  //-----------------------------------------------------------------------------------------------------
  std::vector<GLushort> getConnectedVertices(const std::shared_ptr<SimulatedMesh> &io_meshRef, const GLushort _particle);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve cloth on cloth collisions by calculating line, triangle intersections.
  /// The intersecting points will have their velocities reversed, and their positions reverted to the,
  /// previous ones to simulate a bounce.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  void resolveContinuousCollision_rays(const size_t &_meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve cloth on cloth collisions by representing particles as connected spheres,
  /// and checking whether these spheres intersect. Particles that are too close are projected away from,
  /// eachother.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  void resolveContinuousCollision_spheres(const size_t &_meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve static collision constraints such as spheres an planes that have been set.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  void resolveStaticCollisions(const size_t &_meshIndex);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Forward declare our pointer to implmentation struct.
  //-----------------------------------------------------------------------------------------------------
  struct SolverImpl;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Pointer to the implementation and variables of the solver. This reduces build time and hides,
  /// implementation details.
  //-----------------------------------------------------------------------------------------------------
  std::unique_ptr<SolverImpl> m_impl;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Forward declare our pointer to implmentation of the timer class.
  //-----------------------------------------------------------------------------------------------------
  class FixedTimestepManager;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Pointer to the implementation and variables of the timer. This reduces build time and hides,
  /// implementation details.
  //-----------------------------------------------------------------------------------------------------
  std::unique_ptr<FixedTimestepManager> m_timestepManager;

};

}


#endif // CSBSOLVER_H

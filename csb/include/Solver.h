#ifndef CSBSOLVER_H
#define CSBSOLVER_H

#include <QOpenGLFunctions>
#include <memory>
#include "SimulatedMesh.h"
#include "StaticCollisionConstraint.h"
#include "ContinuousCollisionConstraint.h"

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
  void addSimulatedMesh(const std::shared_ptr<SimulatedMesh> &io_mesh);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Progresses the simulation forwards by the given timestep.
  /// @param _time is the timestep to advance by
  //-----------------------------------------------------------------------------------------------------
  void step(const float _time);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Uses the internal timer to advance the simulation by a fixed timestep.
  //-----------------------------------------------------------------------------------------------------
  void update();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Forwards to the timer function, will either pause or unpause the simulation based on the param.
  /// @param _isPaused is the new state of the timer.
  //-----------------------------------------------------------------------------------------------------
  void setPaused(const bool _isPaused);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Adds a static collision constraint to the simulation.
  /// @param _newConstraint is a raw pointer to the constraint to be added, it is given to an internal,
  /// smart pointer.
  //-----------------------------------------------------------------------------------------------------
  void addStaticCollision(StaticCollisionConstraint* _newConstraint);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Adds a continuous collision constraint to the simulation.
  /// @param _newConstraint is a raw pointer to the constraint to be added, it is given to an internal,
  /// smart pointer.
  //-----------------------------------------------------------------------------------------------------
  void addContinuousCollision(ContinuousCollisionConstraint* _newConstraint);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Adds a force to the simulation, which is added to the current total force.
  /// @param _force is the force to add.
  //-----------------------------------------------------------------------------------------------------
  void addForce(const glm::vec3 &_force);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to access the force being applied to all particles in the sim.
  /// @return the current total force being applied in the sim.
  //-----------------------------------------------------------------------------------------------------
  glm::vec3 getTotalForce() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the total force being applied to all particles in the sim.
  /// @return _force is the force to set.
  //-----------------------------------------------------------------------------------------------------
  void setTotalForce(const glm::vec3 &_force);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the velocity damping in the simulation.
  /// @return _damping is the new damping to apply.
  //-----------------------------------------------------------------------------------------------------
  void setDamping(const float _damping);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to retrieve the velocity damping in the simulation.
  /// @return the current velocity damping.
  //-----------------------------------------------------------------------------------------------------
  float getDamping() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the number of solver iterations.
  /// @param _iterations the amount of times position constraints will be projected every step.
  //-----------------------------------------------------------------------------------------------------
  void setPositionConstraintIterations(const unsigned int _iterations);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to retrieve the current number of solver iterations.
  /// @return the amount of times position constraints will be projected every step.
  //-----------------------------------------------------------------------------------------------------
  unsigned int getPositionConstraintIterations() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the cell offset that is applied when hashing, can be though of as the simulation,
  /// size.
  /// @param _offset is the new cell offset to apply.
  //-----------------------------------------------------------------------------------------------------
  void setCellOffset(const float _offset);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to retrieve the current cell offset.
  /// @return the offset that is applied when hashing, can be though of as the simulation size.
  //-----------------------------------------------------------------------------------------------------
  float getCellOffset() const noexcept;

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
  /// @brief Attempts to resolve static collision constraints such as spheres an planes that have been set.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  void resolveStaticCollisions(const size_t &_meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve continuous collision constraints such as self collisions.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  void resolveContinuousCollisions(const size_t &_meshIndex);

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

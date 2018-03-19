#ifndef CSBMESH_H
#define CSBMESH_H

#include "TriMesh.h"
#include "glm/common.hpp"
#include <tuple>
#include <memory>
#include "Particle.h"
#include "PositionConstraint.h"


namespace csb
{

class SimulatedMesh : public TriMesh
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  SimulatedMesh() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Copy constructor.
  //-----------------------------------------------------------------------------------------------------
  SimulatedMesh(const SimulatedMesh &_rhs);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SimulatedMesh& operator=(const SimulatedMesh &_rhs);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  SimulatedMesh(SimulatedMesh&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SimulatedMesh& operator=(SimulatedMesh&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~SimulatedMesh() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructs our particles using vertex positions, and calculates the shortest/total edge lengths,
  /// should be called prior to being added into, the simulation.
  //-----------------------------------------------------------------------------------------------------
  void init();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Generates structural and bending constraints across all edges and second ring vertices.
  /// @param _bendingStiffness is used to control the stiffness of the bending constraints.
  //-----------------------------------------------------------------------------------------------------
  void generateClothConstraints(const float _bendingStiffness = 0.025f);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to reset the mesh arrays.
  //-----------------------------------------------------------------------------------------------------
  virtual void reset() override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Generates structural constraints across all edges.
  //-----------------------------------------------------------------------------------------------------
  void generateStructuralConstraints();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Generates bending constraints across all second ring neighbour vertices.
  /// @param _bendingStiffness is used to control the stiffness of the bending constraints.
  //-----------------------------------------------------------------------------------------------------
  void generateBendingConstraints(const float _stiffness = 0.025f);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Prrojects all constraints onto the particles.
  //-----------------------------------------------------------------------------------------------------
  void projectConstraints();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Adds the translation to every vertex, this doesn't modify previous position so can affect,
  /// acceleration if performed mid simulation.
  /// @param _translation is the translation to add to the vertices.
  //-----------------------------------------------------------------------------------------------------
  void translate(const glm::vec3 &_translation);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the inverse mass of a particle, this is faster than setParticleMass as it avoids a,
  /// division so use it if you know the inverse mass.
  /// @param _particleIndex is index of the particle, who's inverse mass we should set.
  /// @param _inverseMass is the inverse mass to set.
  //-----------------------------------------------------------------------------------------------------
  void setParticleInverseMass(const GLushort _particleIndex, const float _inverseMass);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the inverse mass of a particle, this is slower than setParticleInverseMass as it,
  /// involves a division so only use it if you do not know the inverse mass.
  /// @param _particleIndex is index of the particle, who's inverse mass we should set.
  /// @param _mass is the inverse mass to set.
  //-----------------------------------------------------------------------------------------------------
  void setParticleMass(const GLushort _particleIndex, const float _mass);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the total edge length, which is used for calculating the average edge length
  /// @return Returns the total edge length of all simulated meshes combined.
  //-----------------------------------------------------------------------------------------------------
  float getTotalEdgeLength() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the shortest edge length, which is used during self collision with sphere resolution.
  /// @return Returns the shortest edge length of all simulated meshes combined.
  //-----------------------------------------------------------------------------------------------------
  float getShortestEdgeLength() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the average edge length, which is used during self collision with sphere resolution.
  /// @return Returns the average edge length of all simulated meshes combined.
  //-----------------------------------------------------------------------------------------------------
  float getAverageEdgeLength() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the shortest edge length, which is used during self collision with sphere resolution.
  /// @return Returns the shortest edge length of all simulated meshes combined.
  //-----------------------------------------------------------------------------------------------------
  void addConstraint(PositionConstraint* _newConstraint);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Friend declaration allows the solver to manipulate particles.
  //-----------------------------------------------------------------------------------------------------
  friend class Solver;
  friend class ContinuousCollisionConstraint;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The constraints on this mesh.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::unique_ptr<PositionConstraint>> m_constraints;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The particles for every vertex in this mesh.
  //-----------------------------------------------------------------------------------------------------
  std::vector<Particle> m_particles;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The shortest edge length in this mesh.
  //-----------------------------------------------------------------------------------------------------
  float m_shortestEdgeLength = 0.0f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The total edge length in this mesh.
  //-----------------------------------------------------------------------------------------------------
  float m_totalEdgeLength = 0.0f;
};

}



#endif // CSBMESH_H

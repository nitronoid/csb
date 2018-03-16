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
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  SimulatedMesh(const SimulatedMesh &_rhs);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
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

  void init();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to reset the mesh arrays.
  //-----------------------------------------------------------------------------------------------------
  virtual void reset() override;
  void generateStructuralConstraints();
  void generateBendingConstraints();
  void projectConstraints();
  void translate(const glm::vec3 &_translation);

  float getTotalEdgeLength() const noexcept;
  float getShortestEdgeLength() const noexcept;

private:
  friend class Solver;

  std::vector<GLushort> getConnectedVertices(const GLushort _vert);

  std::vector<std::unique_ptr<PositionConstraint>> m_constraints;


  std::vector<Particle> m_particles;


  float m_shortestEdgeLength = 0.0f;
  float m_totalEdgeLength = 0.0f;
};

}



#endif // CSBMESH_H

#ifndef CSBMESH_H
#define CSBMESH_H

#include "TriMesh.h"
#include "glm/common.hpp"
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "Particle.h"
#include "Constraint.h"


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
  SimulatedMesh(SimulatedMesh&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SimulatedMesh& operator=(SimulatedMesh&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  ~SimulatedMesh() = default;

  void init();
  void generateStructuralConstraints();
  void generateBendingConstraints();
  void projectConstraints();
  void translate(const glm::vec3 &_translation);

  float getTotalEdgeLength() const noexcept;
  float getShortestEdgeLength() const noexcept;

private:
  friend class Solver;
  struct EdgePair
  {
    EdgePair(const GLushort _a, const GLushort _b) :
      p(std::min(_a, _b), std::max(_a, _b))
    {}
    friend bool operator==(const EdgePair &_a, const EdgePair &_b)
    {
      return _a.p == _b.p;
    }
    std::pair<GLushort, GLushort> p;
  };
  friend struct std::hash<SimulatedMesh::EdgePair>;

  std::unordered_set<EdgePair> getEdges();
  std::vector<GLushort> getConnectedVertices(const GLushort _vert);

  std::vector<std::unique_ptr<Constraint>> m_constraints;


  std::vector<Particle> m_particles;


  float m_shortestEdgeLength = 0.0f;
  float m_totalEdgeLength = 0.0f;
};

}

namespace std
{
template <>
struct hash<csb::SimulatedMesh::EdgePair>
{
  size_t operator()(const csb::SimulatedMesh::EdgePair &_key) const
  {
    return std::hash<size_t>()(std::hash<GLushort>()(_key.p.first)) ^ std::hash<GLushort>()(_key.p.second);
  }
};
}

#endif // CSBMESH_H

#ifndef CSBMESH_H
#define CSBMESH_H

#include "Mesh.h"
#include "glm/common.hpp"
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include "CSBparticle.h"
#include "CSBconstraint.h"

class CSBmesh : public Mesh
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  CSBmesh() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  CSBmesh(CSBmesh&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  CSBmesh& operator=(CSBmesh&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  ~CSBmesh() = default;

  void init();
  void generateStructuralConstraints();
  void generateBendingConstraints();
  void projectConstraints();
  void translate(const glm::vec3 &_translation);

  float getTotalEdgeLength() const noexcept;
  float getShortestEdgeLength() const noexcept;

private:
  friend class CSBsolver;
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
  friend struct std::hash<CSBmesh::EdgePair>;

  std::unordered_set<EdgePair> getEdges();
  std::vector<GLushort> getConnectedVertices(const GLushort _vert);

  std::vector<std::unique_ptr<CSBconstraint>> m_constraints;


  std::vector<CSBparticle> m_particles;


  float m_shortestEdgeLength = 0.0f;
  float m_totalEdgeLength = 0.0f;
};



namespace std
{
template <>
struct hash<CSBmesh::EdgePair>
{
  size_t operator()(const CSBmesh::EdgePair &_key) const
  {
    return std::hash<size_t>()(std::hash<GLushort>()(_key.p.first)) ^ std::hash<GLushort>()(_key.p.second);
  }
};
}

#endif // CSBMESH_H

#ifndef CSBSOLVER_H
#define CSBSOLVER_H

#include <QOpenGLFunctions>
#include "glm/common.hpp"
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "CSBparticle.h"
#include "CSBconstraint.h"
#include "Mesh.h"

class CSBsolver
{
public:
  void addTriangleMesh(Mesh& _mesh);
  void update(const float _time);

private:
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
  friend struct std::hash<CSBsolver::EdgePair>;

  void hashVerts();
  void hashTris();
  std::unordered_set<EdgePair> getEdges(Mesh* _meshRef);
  std::vector<GLushort> getConnectedVertices(Mesh* _meshRef, const GLushort _vert);

  std::vector<CSBparticle> m_particles;
  std::vector<std::unique_ptr<CSBconstraint>> m_constraints;
  std::vector<Mesh*> m_referencedMeshes;

  glm::ivec3 calcCell(const glm::vec3& _coord) const;
  size_t hashCell (const glm::ivec3& _cell) const;
  size_t hashParticle(const glm::vec3& _coord) const;
  void resolveSelfCollision_rays();

  void resolveSelfCollision_spheres();

  std::vector<std::vector<GLushort>> m_hashTable;
  std::vector<std::vector<size_t>> m_triangleVertHash;
  std::vector<GLushort> m_indices;

  float m_shortestEdgeDist = 0.0f;
  float m_avgEdgeLength = 0.0f;
};


namespace std
{
template <>
struct hash<CSBsolver::EdgePair>
{
  size_t operator()(const CSBsolver::EdgePair &_key) const
  {
    return std::hash<size_t>()(std::hash<GLushort>()(_key.p.first)) ^ std::hash<GLushort>()(_key.p.second);
  }
};
}


#endif // CSBSOLVER_H

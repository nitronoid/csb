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
#include "CSBmesh.h"

class CSBsolver
{
public:
  void addTriangleMesh(CSBmesh& _mesh);
  void update(const float _time);

private:

  void hashVerts(const size_t& _meshIndex);
  void hashTris(const size_t &_meshIndex);
  std::vector<GLushort> getConnectedVertices(CSBmesh* _meshRef, const GLushort _vert);

  std::vector<CSBmesh*> m_referencedMeshes;

  glm::ivec3 calcCell(const glm::vec3& _coord) const;
  size_t hashCell (const glm::ivec3& _cell) const;
  size_t hashParticle(const glm::vec3& _coord) const;
  void resolveContinuousCollision_rays(const size_t &_meshIndex);

  void resolveContinuousCollision_spheres(const size_t &_meshIndex);

  std::vector<std::vector<std::pair<GLushort, GLushort>>> m_hashTable;
  std::vector<std::vector<size_t>> m_triangleVertHash;
  std::vector<size_t> m_vertHashOffset;
  std::vector<size_t> m_triHashOffset;

  size_t m_numParticles = 0;
  size_t m_numTris = 0;
  size_t m_numEdges = 0;

  float m_shortestEdgeDist = std::numeric_limits<float>::max();
  float m_avgEdgeLength = 0.0f;
  float m_totalEdgeLength = 0.0f;
};


#endif // CSBSOLVER_H

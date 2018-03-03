#ifndef CSBMESH_H
#define CSBMESH_H

#include "Mesh.h"
#include <tuple>
#include <unordered_set>
#include "csbpoint.h"
#include "CSBconstraint.h"

class CSBmesh : public Mesh
{
public:
  void init();
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
  friend struct std::hash<CSBmesh::EdgePair>;


  std::unordered_set<EdgePair> getEdges();
  std::vector<GLushort> getConnectedVertices(const GLushort _vert);

  std::vector<CSBpoint> m_points;
  std::vector<std::unique_ptr<CSBconstraint>> m_constraints;
};

namespace std
{
template <>
struct hash<CSBmesh::EdgePair>
{
  size_t operator()(const CSBmesh::EdgePair &_up) const
  {
    return std::hash<size_t>()(std::hash<GLushort>()(_up.p.first)) ^ std::hash<GLushort>()(_up.p.second);
  }
};
}

#endif // CSBMESH_H

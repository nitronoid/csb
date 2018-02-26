#ifndef CSBMESH_H
#define CSBMESH_H

#include "Mesh.h"
#include "CSBconstraint.h"
#include <tuple>
#include <unordered_set>

class CSBmesh : public Mesh
{
public:
  void init();
  void update(const float _time);

private:


  struct edgePair
  {
    edgePair(const GLushort _a, const GLushort _b) :
      p(std::min(_a, _b), std::max(_a, _b))
    {}
    friend bool operator==(const edgePair &_a, const edgePair &_b)
    {
      return _a.p == _b.p;
    }
    std::pair<GLushort, GLushort> p;
  };
  friend struct std::hash<CSBmesh::edgePair>;


  std::unordered_set<edgePair> getEdges();

  std::vector<glm::vec3> m_prevPosition;
  std::vector<std::unique_ptr<CSBconstraint>> m_constraints;
};

namespace std
{
template <>
struct hash<CSBmesh::edgePair>
{
  size_t operator()(const CSBmesh::edgePair &_up) const
  {
    return std::hash<size_t>()(std::hash<GLushort>()(_up.p.first)) ^ std::hash<GLushort>()(_up.p.second);
  }
};
}

#endif // CSBMESH_H

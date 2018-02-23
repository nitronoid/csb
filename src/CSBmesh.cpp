#include "CSBmesh.h"
#include "gtx/fast_square_root.hpp"
#include <tuple>
#include <unordered_set>

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

namespace std
{
template <>
struct hash<edgePair>
{
  size_t operator()(const edgePair &_up) const
  {
    return std::hash<size_t>()(std::hash<GLushort>()(_up.p.first)) ^ std::hash<GLushort>()(_up.p.second);
  }
};
}

void CSBmesh::init()
{
  std::unordered_set<edgePair> edgeSet;
  auto numEdges = m_vertices.size() + m_indices.size() / 3 - 2;
  edgeSet.reserve(numEdges);

  m_prevPosition = m_vertices;
  for (size_t i = 0; i < m_indices.size() - 2; i+=3)
  {
    auto p1 = m_indices[i];
    auto p2 = m_indices[i + 1];
    auto p3 = m_indices[i + 2];
    edgeSet.insert({p1, p2});
    edgeSet.insert({p2, p3});
    edgeSet.insert({p3, p1});
  }

  for (const auto & edge : edgeSet)
  {
    auto p1 = edge.p.first;
    auto p2 = edge.p.second;
    float distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_constraints.emplace_back(new DistanceConstraint(p1, p2, distance));
  }
}

void CSBmesh::update(const float _time)
{
  const auto pos = m_vertices[0];
  const auto gravity = glm::vec3(0.f,-0.1f,0.f);
  for (size_t i = 0; i < m_vertices.size(); ++i)
  {
    auto& pos = m_vertices[i];
    glm::vec3 newPos = pos * 2.0f - m_prevPosition[i] + gravity * _time * _time;
    m_prevPosition[i] = pos;
    pos = newPos;
  }

  //for (int i = 0; i < 3; ++i)
  for (auto& constraint : m_constraints)
  {
    constraint->project(m_vertices);
  }

  m_vertices[0] = pos;
}

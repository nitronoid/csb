#include "CSBmesh.h"
#include "gtx/fast_square_root.hpp"

std::unordered_set<CSBmesh::EdgePair> CSBmesh::getEdges()
{
  std::unordered_set<EdgePair> edgeSet;
  auto numEdges = m_vertices.size() + (m_indices.size() / 3) - 2;
  edgeSet.reserve(numEdges);

  const auto last = m_indices.size() - 2;
  for (size_t i = 0; i < last; i+=3)
  {
    const auto p1 = m_indices[i];
    const auto p2 = m_indices[i + 1];
    const auto p3 = m_indices[i + 2];
    edgeSet.insert({p1, p2});
    edgeSet.insert({p2, p3});
    edgeSet.insert({p3, p1});
  }
  return edgeSet;
}


std::vector<GLushort> CSBmesh::getConnectedVertices(const GLushort _vert)
{
  std::unordered_set<GLushort> connected;

  const auto last = m_indices.size() - 2;
  for (size_t i = 0; i < last; i+=3)
  {
    std::unordered_set<GLushort> facePoints = {m_indices[i], m_indices[i + 1], m_indices[i + 2]};

    if (facePoints.count(_vert))
    {
      facePoints.erase(_vert);
      connected.insert(facePoints.begin(), facePoints.end());
    }
  }

  return std::vector<GLushort>{connected.begin(), connected.end()};
}

void CSBmesh::init()
{
  m_points.reserve(m_vertices.size());
  for (auto& vert : m_vertices)
    m_points.emplace_back(vert, 1.f);


  m_points[0].m_invMass = 0.f;
  m_points[60].m_invMass = 0.f;

  auto edgeSet = getEdges();
  for (const auto & edge : edgeSet)
  {
    auto p1 = edge.p.first;
    auto p2 = edge.p.second;
    float distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_constraints.emplace_back(new DistanceConstraint(p1, p2, distance));
  }

  const auto size = m_vertices.size();
  std::unordered_set<EdgePair> connections;
  for (GLushort v = 0; v < size; ++v)
  {
    auto neighbours = getConnectedVertices(v);
    for (const auto vi : neighbours)
    {
      float bestCosTheta = 0.0f;
      auto bestV = vi;
      for (const auto vj : neighbours)
      {
        auto a = m_vertices[vi] - m_vertices[v];
        auto b = m_vertices[vj] - m_vertices[v];
        auto cosTheta = glm::dot(a, b) / (glm::fastLength(a) * glm::fastLength(b));
        if (cosTheta < bestCosTheta)
        {
          bestCosTheta = cosTheta;
          bestV = vj;
        }
      }
      EdgePair connection {bestV, vi};
      if (!connections.count(connection))
      {
        connections.insert(connection);
        constexpr float third = 1.0f / 3.0f;
        auto centre = third * (m_vertices[vi] + m_vertices[bestV] + m_vertices[v]);
        auto rest = glm::fastDistance(m_vertices[v], centre);
        m_constraints.emplace_back(new BendingConstraint(vi, bestV, v, rest, m_points));
      }
    }
  }
}

void CSBmesh::update(const float _time)
{
  const auto gravity = glm::vec3(0.f,-4.95f,0.f);
  const auto size = m_points.size();
  for (size_t i = 0; i < size; ++i)
  {
    auto& point = m_points[i];
    glm::vec3 newPos = point.m_pos * 2.0f - point.m_prevPos + (point.m_invMass * gravity * _time * _time);
    point.m_prevPos = point.m_pos;
    point.m_pos = newPos;
  }

  for (int i = 0; i < 3; ++i)
  for (auto& constraint : m_constraints)
  {
    constraint->project(m_points);
  }
}

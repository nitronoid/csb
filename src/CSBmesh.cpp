#include "CSBmesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/fast_square_root.hpp"
#include "gtx/norm.hpp"
#include "gtx/normal.hpp"
#include "gtx/intersect.hpp"
//#undef GLM_ENABLE_EXPERIMENTAL
#include <random>
#include <algorithm>


float CSBmesh::getTotalEdgeLength() const noexcept
{
  return m_totalEdgeLength;
}

float CSBmesh::getShortestEdgeLength() const noexcept
{
  return m_shortestEdgeLength;
}

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
  return m_adjacency[_vert];
}

void CSBmesh::init()
{
  for (auto& vert : m_vertices)
    m_particles.emplace_back(vert, 1.f);

  m_particles[0].m_invMass = 0.f;
  m_particles[m_particles.size() - 1].m_invMass = 0.f;

  auto edgeSet = getEdges();
  const auto& firstEdge = edgeSet.begin()->p;
  m_shortestEdgeLength = glm::fastDistance(m_vertices[m_indices[firstEdge.first]], m_vertices[m_indices[firstEdge.second]]);
  for (const auto & edge : edgeSet)
  {
    const auto p1 = edge.p.first;
    const auto p2 = edge.p.second;
    const auto distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_shortestEdgeLength = std::min(m_shortestEdgeLength, distance);
    m_totalEdgeLength += distance;
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
        if (vj == vi) continue;
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
        static constexpr float third = 1.0f / 3.0f;
        auto centre = third * (m_vertices[vi] + m_vertices[bestV] + m_vertices[v]);
        auto rest = glm::fastDistance(m_vertices[v], centre);
        m_constraints.emplace_back(new BendingConstraint(vi, bestV, v, rest, m_particles));
      }
    }
  }
}

void CSBmesh::projectConstraints()
{
  for (int i = 0; i < 30; ++i)
    for (auto& constraint : m_constraints)
    {
      constraint->project(m_particles);
    }
}

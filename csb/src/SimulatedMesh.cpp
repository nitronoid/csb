#include "SimulatedMesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/fast_square_root.hpp"
#include "gtx/norm.hpp"
#include "gtx/normal.hpp"
#include "gtx/intersect.hpp"
//#undef GLM_ENABLE_EXPERIMENTAL
#include <random>
#include <algorithm>

csb::SimulatedMesh::SimulatedMesh(const csb::SimulatedMesh &_rhs) :
  csb::TriMesh(_rhs)
{
  m_particles = _rhs.m_particles;
  m_shortestEdgeLength = _rhs.m_shortestEdgeLength;
  m_totalEdgeLength = _rhs.m_totalEdgeLength;
  for (const auto& constraint : _rhs.m_constraints)
    m_constraints.emplace_back(constraint->clone());
}

csb::SimulatedMesh& csb::SimulatedMesh::operator=(const csb::SimulatedMesh &_rhs)
{
  TriMesh::operator=(_rhs);
  m_particles = _rhs.m_particles;
  m_shortestEdgeLength = _rhs.m_shortestEdgeLength;
  m_totalEdgeLength = _rhs.m_totalEdgeLength;
  for (const auto& constraint : _rhs.m_constraints)
    m_constraints.emplace_back(constraint->clone());

  return *this;
}

void csb::SimulatedMesh::reset()
{
  TriMesh::reset();
  m_constraints.clear();
  m_shortestEdgeLength = 0.f;
  m_totalEdgeLength = 0.f;
}

float csb::SimulatedMesh::getTotalEdgeLength() const noexcept
{
  return m_totalEdgeLength;
}

float csb::SimulatedMesh::getShortestEdgeLength() const noexcept
{
  return m_shortestEdgeLength;
}

std::vector<GLushort> csb::SimulatedMesh::getConnectedVertices(const GLushort _vert)
{
  return m_adjacency[_vert];
}

void csb::SimulatedMesh::init()
{
  for (auto& vert : m_vertices)
    m_particles.emplace_back(vert, 1.f);

  m_particles[0].m_invMass = 0.f;
  m_particles[m_particles.size() - 1].m_invMass = 0.f;

  generateStructuralConstraints();

  generateBendingConstraints();
}

void csb::SimulatedMesh::generateStructuralConstraints()
{
  const auto& firstEdge = m_edges[0].p;
  m_shortestEdgeLength = glm::fastDistance(m_vertices[m_indices[firstEdge.first]], m_vertices[m_indices[firstEdge.second]]);
  for (const auto & edge : m_edges)
  {
    const auto p1 = edge.p.first;
    const auto p2 = edge.p.second;
    const auto distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_shortestEdgeLength = std::min(m_shortestEdgeLength, distance);
    m_totalEdgeLength += distance;
    m_constraints.emplace_back(new DistanceConstraint(p1, p2, distance));
  }
}

void csb::SimulatedMesh::generateBendingConstraints()
{
  const auto size = m_vertices.size();
  std::unordered_set<Edge> connections;
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
      Edge connection {bestV, vi};
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

void csb::SimulatedMesh::projectConstraints()
{
  for (int i = 0; i < 30; ++i)
    for (auto& constraint : m_constraints)
    {
      constraint->project(m_particles);
    }
}

void csb::SimulatedMesh::translate(const glm::vec3 &_translation)
{
  for (auto& v : m_vertices)
    v += _translation;
}

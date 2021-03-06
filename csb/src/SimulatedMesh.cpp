#include "SimulatedMesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/fast_square_root.hpp"
#include "gtx/norm.hpp"
#include "gtx/normal.hpp"
#include "gtx/intersect.hpp"
#undef GLM_ENABLE_EXPERIMENTAL
#include <random>
#include <algorithm>
#include "BendingConstraint.h"
#include "DistanceConstraint.h"
#include "SphereCollisionConstraint.h"


//----------------------------------------------------------------------------------------------------------------------------
csb::SimulatedMesh::SimulatedMesh(const csb::SimulatedMesh &_rhs) :
  csb::TriMesh(_rhs),
  m_particles(_rhs.m_particles),
  m_shortestEdgeLength(_rhs.m_shortestEdgeLength),
  m_totalEdgeLength(_rhs.m_totalEdgeLength)
{
  // Clone all the constraints using dispatch
  for (const auto& constraint : _rhs.m_constraints)
    m_constraints.emplace_back(constraint->clone());
}
//----------------------------------------------------------------------------------------------------------------------------
csb::SimulatedMesh& csb::SimulatedMesh::operator=(const csb::SimulatedMesh &_rhs)
{
  TriMesh::operator=(_rhs);
  m_particles = _rhs.m_particles;
  m_shortestEdgeLength = _rhs.m_shortestEdgeLength;
  m_totalEdgeLength = _rhs.m_totalEdgeLength;
  // Clone all the constraints using dispatch
  for (const auto& constraint : _rhs.m_constraints)
    m_constraints.emplace_back(constraint->clone());

  // reference to self
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::reset()
{
  TriMesh::reset();
  m_constraints.clear();
  m_shortestEdgeLength = 0.f;
  m_totalEdgeLength = 0.f;
}
//----------------------------------------------------------------------------------------------------------------------------
float csb::SimulatedMesh::getTotalEdgeLength() const noexcept
{
  return m_totalEdgeLength;
}
//----------------------------------------------------------------------------------------------------------------------------
float csb::SimulatedMesh::getAverageEdgeLength() const noexcept
{
  return m_totalEdgeLength / m_edges.size();
}
//----------------------------------------------------------------------------------------------------------------------------
float csb::SimulatedMesh::getShortestEdgeLength() const noexcept
{
  return m_shortestEdgeLength;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::init()
{
  // Construct all particles from vertices
  for (auto& vert : m_vertices)
    m_particles.emplace_back(vert, 1.f);

  // Calculate the shortest edge and total edge distances.
  const auto& firstEdge = m_edges[0].p;
  m_shortestEdgeLength = glm::fastDistance(m_vertices[m_indices[firstEdge.first]], m_vertices[m_indices[firstEdge.second]]);
  for (const auto & edge : m_edges)
  {
    const auto p1 = edge.p.first;
    const auto p2 = edge.p.second;
    const auto distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_shortestEdgeLength = std::min(m_shortestEdgeLength, distance);
    m_totalEdgeLength += distance;
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::generateClothConstraints(const float _bendingStiffness)
{
  // Convenience function to create structural and bending constraints
  generateStructuralConstraints();
  generateBendingConstraints(_bendingStiffness);
}

void csb::SimulatedMesh::generateStructuralConstraints()
{
  // Create distance constraints for every edge, which matches that edge length
  for (const auto & edge : m_edges)
  {
    const auto p1 = edge.p.first;
    const auto p2 = edge.p.second;
    const auto distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_constraints.emplace_back(new DistanceConstraint(p1, p2, distance));
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::generateBendingConstraints(const float _stiffness)
{
  const auto size = m_vertices.size();
  std::unordered_set<Edge> connections;
  // Create bending constraints over triplets of vertices in the same neighbourhood
  for (GLushort v = 0; v < size; ++v)
  {
    // Get the vertices adjacent vertices
    auto neighbours = m_adjacency[v];
    for (const auto vi : neighbours)
    {
      // Choose the neighbour with the highest angle
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
      // make sure we haven't used the same two vertices
      Edge connection {bestV, vi};
      if (!connections.count(connection))
      {
        connections.insert(connection);
        static constexpr float third = 1.0f / 3.0f;
        auto centre = third * (m_vertices[vi] + m_vertices[bestV] + m_vertices[v]);
        auto rest = glm::fastDistance(m_vertices[v], centre);
        m_constraints.emplace_back(new BendingConstraint(vi, bestV, v, rest, _stiffness, m_particles));
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::addConstraint(PositionConstraint* _newConstraint)
{
  m_constraints.emplace_back(_newConstraint);
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::projectConstraints()
{
  // project all constraints
  for (auto& constraint : m_constraints)
  {
    constraint->project(m_particles);
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::translate(const glm::vec3 &_translation)
{
  // Apply to all verts
  for (auto& p : m_particles)
  {
    *p.m_pos += _translation;
    p.m_prevPos += _translation;
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::setParticleInverseMass(const GLushort _particleIndex, const float _inverseMass)
{
  m_particles[_particleIndex].m_invMass = _inverseMass;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SimulatedMesh::setParticleMass(const GLushort _particleIndex, const float _mass)
{
  m_particles[_particleIndex].m_invMass = 1.0f / _mass;
}
//----------------------------------------------------------------------------------------------------------------------------

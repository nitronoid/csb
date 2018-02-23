#include "CSBmesh.h"
#include "gtx/fast_square_root.hpp"

void CSBmesh::init()
{
  m_prevPosition = m_vertices;
  for (size_t i = 0; i < m_indices.size() - 1; ++i)
  {
    auto p1 = m_indices[i];
    auto p2 = m_indices[i + 1];
    if (p1 == p2) continue;
    float distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_constraints.emplace_back(new DistanceConstraint(p1, p2, distance));
  }
}

void CSBmesh::update(const float _time)
{
  constexpr auto gravity = glm::vec3(0.f,-0.1f,0.f);
  for (size_t i = 0; i < m_vertices.size(); ++i)
  {
    auto& pos = m_vertices[i];
    glm::vec3 newPos = pos * 2.0f - m_prevPosition[i] + gravity * _time * _time;
    m_prevPosition[i] = pos;
    pos = newPos;
  }

  for (int i = 0; i < 3; ++i)
  for (auto& constraint : m_constraints)
  {
    constraint->project(m_vertices);
  }
}

#include "CSBmesh.h"
#include "gtx/fast_square_root.hpp"

void CSBmesh::init()
{
  m_prevPosition = m_vertices;
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

  constexpr auto restLength = 1.0f;
  constexpr auto restSqr = restLength * restLength;
  for (size_t i = 0; i < m_vertices.size() - 1; ++i)
  {
    auto& p1 = m_vertices[i];
    auto& p2 = m_vertices[i + 1];
    auto delta = p2 - p1;
    delta *= restSqr / (delta * delta + restSqr) - 0.5f;
    p1 -= delta;
    p2 += delta;
  }
}

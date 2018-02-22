#include "CSBmesh.h"

void CSBmesh::init()
{
  m_prevPosition = m_vertices;
}

void CSBmesh::update(const float _time)
{
  constexpr auto gravity = glm::vec3(0.f,-0.1f,0.f);
  for (size_t i = 0; i < m_vertices.size(); ++i)
  {
    glm::vec3 newPos = m_vertices[i] * 2.0f - m_prevPosition[i] + gravity * _time * _time;
    m_prevPosition[i] = m_vertices[i];
    m_vertices[i] = newPos;
  }
}

#ifndef CSBPOINT_H
#define CSBPOINT_H

#include "vec3.hpp"

struct CSBparticle
{
  CSBparticle(glm::vec3 &_pos, const float &_invMass) :
      m_pos(_pos),
      m_prevPos(_pos),
      m_invMass(_invMass)
  {}
  glm::vec3& m_pos;
  glm::vec3 m_prevPos;
  float m_invMass;
};

#endif // CSBPOINT_H

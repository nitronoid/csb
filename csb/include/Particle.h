#ifndef CSBPOINT_H
#define CSBPOINT_H

#include "vec3.hpp"


namespace csb
{

struct Particle
{
  Particle() = default;
  Particle(const Particle&) = default;
  Particle& operator=(const Particle&) = default;
  Particle(Particle&&) = default;
  Particle& operator=(Particle&&) = default;
  ~Particle() = default;
  Particle(glm::vec3 &_pos, const float &_invMass) :
      m_pos(&_pos),
      m_prevPos(_pos),
      m_invMass(_invMass)
  {}

  glm::vec3* m_pos = nullptr;
  glm::vec3 m_prevPos{0.f};
  float m_invMass{0.f};
};

}

#endif // CSBPOINT_H

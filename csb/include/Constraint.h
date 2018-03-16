#ifndef CSBCONSTRAINT_H
#define CSBCONSTRAINT_H

#include <vector>
#include <array>
#include "vec3.hpp"
#include "Particle.h"

namespace csb
{

class Constraint
{
public:
  Constraint() = default;
  Constraint(const Constraint&) = default;
  Constraint& operator=(const Constraint&) = default;
  Constraint(Constraint&&) = default;
  Constraint& operator=(Constraint&&) = default;
  virtual ~Constraint();
  virtual void project(std::vector<Particle> &_positions) = 0;
};

}

namespace csb
{

class DistanceConstraint : public Constraint
{
public:
  DistanceConstraint(const size_t _p1, const size_t _p2, float _rest) :
    m_rest(_rest),
    m_p1(_p1),
    m_p2(_p2)
  {}
  DistanceConstraint(const DistanceConstraint&) = default;
  DistanceConstraint& operator=(const DistanceConstraint&) = default;
  DistanceConstraint(DistanceConstraint&&) = default;
  DistanceConstraint& operator=(DistanceConstraint&&) = default;
  virtual ~DistanceConstraint() override = default;
  virtual void project(std::vector<Particle> &_positions) override;

private:
  float m_rest;
  size_t m_p1;
  size_t m_p2;
};

}

namespace csb
{

class BendingConstraint : public Constraint
{
public:
  BendingConstraint(const size_t _p1, const size_t _p2, const size_t _p3, float _rest, const std::vector<Particle>&_points) :
    m_p({{_p1, _p2, _p3}}),
    m_rest(_rest)
  {
    auto W = _points[_p1].m_invMass + _points[_p2].m_invMass + 2.f * _points[_p3].m_invMass;
    for (unsigned int i = 0; i < m_w.size(); ++i)
      m_w[i] = _points[m_p[i]].m_invMass / W;
  }

  BendingConstraint(const BendingConstraint&) = default;
  BendingConstraint& operator=(const BendingConstraint&) = default;
  BendingConstraint(BendingConstraint&&) = default;
  BendingConstraint& operator=(BendingConstraint&&) = default;
  virtual ~BendingConstraint() override = default;
  virtual void project(std::vector<Particle> &_positions) override;

private:
  std::array<size_t, 3> m_p;
  std::array<float, 3> m_w;
  float m_rest;
};

}

namespace csb
{

class PinConstraint : public Constraint
{
public:
  PinConstraint(const size_t &_p, const glm::vec3 &_pin) :
    m_pin(_pin),
    m_p(_p)
  {}
  PinConstraint(const PinConstraint&) = default;
  PinConstraint& operator=(const PinConstraint&) = default;
  PinConstraint(PinConstraint&&) = default;
  PinConstraint& operator=(PinConstraint&&) = default;
  virtual ~PinConstraint() override = default;
  virtual void project(std::vector<Particle> &_positions) override;

private:
  glm::vec3 m_pin;
  size_t m_p;
};

}

namespace csb
{

class SelfCollisionConstraint : public Constraint
{
public:
  SelfCollisionConstraint(const glm::vec3 &_intersectionP, const size_t &_p, const size_t &_t0, const size_t &_t1, const size_t &_t2, const std::vector<Particle>&_points) :
    m_t({{_t0, _t1, _t2}}),
    m_intersectionP(_intersectionP),
    m_p(_p)
  {
    static constexpr float third = 1.f / 3.f;
    const auto W = (_points[_t0].m_invMass + _points[_t1].m_invMass + _points[_t2].m_invMass) * third;
    for (unsigned int i = 0; i < m_w.size(); ++i)
      m_w[i] = _points[m_t[i]].m_invMass / W;

  }
  SelfCollisionConstraint(const SelfCollisionConstraint&) = default;
  SelfCollisionConstraint& operator=(const SelfCollisionConstraint&) = default;
  SelfCollisionConstraint(SelfCollisionConstraint&&) = default;
  SelfCollisionConstraint& operator=(SelfCollisionConstraint&&) = default;
  virtual ~SelfCollisionConstraint() override = default;
  virtual void project(std::vector<Particle> &_positions) override;

private:
  std::array<size_t, 3> m_t;
  std::array<float, 3> m_w;
  glm::vec3 m_intersectionP;
  size_t m_p;
};

}



#endif // CSBCONSTRAINT_H

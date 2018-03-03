#ifndef CSBCONSTRAINT_H
#define CSBCONSTRAINT_H

#include <vector>
#include <array>
#include "vec3.hpp"
#include "csbpoint.h"

class CSBconstraint
{
public:
  CSBconstraint() = default;
  CSBconstraint(const CSBconstraint&) = default;
  CSBconstraint& operator=(const CSBconstraint&) = default;
  CSBconstraint(CSBconstraint&&) = default;
  CSBconstraint& operator=(CSBconstraint&&) = default;
  virtual ~CSBconstraint();
  virtual void project(std::vector<CSBpoint> &_positions) = 0;
};

class DistanceConstraint : public CSBconstraint
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
  virtual void project(std::vector<CSBpoint> &_positions) override;

private:
  float m_rest;
  size_t m_p1;
  size_t m_p2;
};


class BendingConstraint : public CSBconstraint
{
public:
  BendingConstraint(const size_t _p1, const size_t _p2, const size_t _p3, float _rest, const std::vector<CSBpoint>&_points) :
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
  virtual void project(std::vector<CSBpoint> &_positions) override;

private:
  std::array<size_t, 3> m_p;
  std::array<float, 3> m_w;
  float m_rest;
};

class PinConstraint : public CSBconstraint
{
public:
  PinConstraint(const size_t _p, const glm::vec3 &_pin) :
    m_pin(_pin),
    m_p(_p)
  {}
  PinConstraint(const PinConstraint&) = default;
  PinConstraint& operator=(const PinConstraint&) = default;
  PinConstraint(PinConstraint&&) = default;
  PinConstraint& operator=(PinConstraint&&) = default;
  virtual ~PinConstraint() override = default;
  virtual void project(std::vector<CSBpoint> &_positions) override;

private:
  glm::vec3 m_pin;
  size_t m_p;
};


#endif // CSBCONSTRAINT_H

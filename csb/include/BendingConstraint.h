#ifndef BENDINGCONSTRAINT_H
#define BENDINGCONSTRAINT_H

#include "PositionConstraint.h"
#include <array>

namespace csb
{

class BendingConstraint : public PositionConstraint
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
  virtual void project(std::vector<Particle> &io_particles) override;
  virtual PositionConstraint* clone() const override;

private:
  std::array<size_t, 3> m_p;
  std::array<float, 3> m_w;
  float m_rest;
};

}

#endif // BENDINGCONSTRAINT_H

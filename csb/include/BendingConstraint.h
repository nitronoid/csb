#ifndef BENDINGCONSTRAINT_H
#define BENDINGCONSTRAINT_H

#include "PositionConstraint.h"
#include <array>

namespace csb
{

class BendingConstraint : public PositionConstraint
{
public:
  BendingConstraint() = default;
  BendingConstraint(
      const size_t _p1,
      const size_t _p2,
      const size_t _p3,
      const float _rest,
      const float _stiffness,
      const std::vector<Particle>&_particles
      );

  BendingConstraint(const BendingConstraint&) = default;
  BendingConstraint& operator=(const BendingConstraint&) = default;
  BendingConstraint(BendingConstraint&&) = default;
  BendingConstraint& operator=(BendingConstraint&&) = default;
  virtual ~BendingConstraint() override = default;
  virtual void project(std::vector<Particle> &io_particles) override;
  virtual PositionConstraint* clone() const override;

  size_t getParticleIndex(const unsigned short _index) const noexcept;
  float  getParticleWeight(const unsigned short _index) const noexcept;
  float  getRest() const noexcept;
  float  getStiffness() const noexcept;

  void setParticleIndex(const unsigned short _index, const size_t &_p);
  void setParticleWeight(const unsigned short _index, const float _invMass);
  void setRest(const float _rest);
  void setStiffness(const float _stiffness);

private:
  std::array<size_t, 3> m_p {{0ul, 0ul, 0ul}};
  std::array<float, 3>  m_w {{0.f, 0.f, 0.f}};
  float m_rest = 0.f;
  float m_stiffness = 0.025f;
};

}

#endif // BENDINGCONSTRAINT_H

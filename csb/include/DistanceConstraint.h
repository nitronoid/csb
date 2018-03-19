#ifndef DISTANCECONSTRAINT_H
#define DISTANCECONSTRAINT_H

#include "PositionConstraint.h"

namespace csb
{

class DistanceConstraint : public PositionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  DistanceConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor.
  /// @param _p1 is the first particle index that this constraint will be projected onto.
  /// @param _p2 is the second particle index that this constraint will be projected onto.
  /// @param _rest is distance from _p1 to _p2 that satisfies the constraint.
  //-----------------------------------------------------------------------------------------------------
  DistanceConstraint(const size_t _p1, const size_t _p2, float _rest) :
    m_rest(_rest),
    m_p1(_p1),
    m_p2(_p2)
  {}
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  DistanceConstraint(const DistanceConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  DistanceConstraint& operator=(const DistanceConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  DistanceConstraint(DistanceConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  DistanceConstraint& operator=(DistanceConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~DistanceConstraint() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles. Attempts to make the distance between the,
  /// points equal to the rest distance.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(std::vector<Particle> &io_particles) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual PositionConstraint* clone() const override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the rest distance between the two particles.
  /// @return The rest distance between the two particles.
  //-----------------------------------------------------------------------------------------------------
  float  getRest() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the particle index of the first particle.
  /// @return The particle index m_p1.
  //-----------------------------------------------------------------------------------------------------
  size_t getParticleIndex1() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the particle index of the second particle.
  /// @return The particle index m_p2.
  //-----------------------------------------------------------------------------------------------------
  size_t getParticleIndex2() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the rest distance between the two particles.
  /// @param _rest is the new rest distance between the two particles.
  //-----------------------------------------------------------------------------------------------------
  void setRest(const float _rest);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the particle index of the first particle.
  /// @param _p1 is the new particle index for m_p1.
  //-----------------------------------------------------------------------------------------------------
  void setParticleIndex1(const size_t &_p1);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the particle index of the second particle.
  /// @param _p2 is the new particle index for m_p2.
  //-----------------------------------------------------------------------------------------------------
  void setParticleIndex2(const size_t &_p2);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief The rest distance between m_p1 and m_p2.
  //-----------------------------------------------------------------------------------------------------
  float m_rest = 0.f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The index of the first particle.
  //-----------------------------------------------------------------------------------------------------
  size_t m_p1 = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The index of the second particle.
  //-----------------------------------------------------------------------------------------------------
  size_t m_p2 = 0;
};

}

#endif // DISTANCECONSTRAINT_H

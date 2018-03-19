#ifndef PINCONSTRAINT_H
#define PINCONSTRAINT_H

#include "PositionConstraint.h"

namespace csb
{

class PinConstraint : public PositionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  PinConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor.
  /// @param _p is the particle index that will be pinned.
  /// @param _pin is the position to pin the particle to.
  //-----------------------------------------------------------------------------------------------------
  PinConstraint(const size_t &_p, const glm::vec3 &_pin) :
    m_pin(_pin),
    m_p(_p)
  {}
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  PinConstraint(const PinConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  PinConstraint& operator=(const PinConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  PinConstraint(PinConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  PinConstraint& operator=(PinConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~PinConstraint() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles. Sets the particle's position to m_pin.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(std::vector<Particle> &io_particles) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual PositionConstraint* clone() const override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the index of the particle that is pinned.
  /// @return The index of the pinned particle.
  //-----------------------------------------------------------------------------------------------------
  size_t getParticleIndex() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the position that the particle is pinned to.
  /// @return The pin position.
  //-----------------------------------------------------------------------------------------------------
  glm::vec3 getPinPosition() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the index so that a new particle is pinned.
  /// @param _p is the index of the new particle to pin.
  //-----------------------------------------------------------------------------------------------------
  void setParticleIndex(const size_t &_p);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the position that the particle should be pinned to.
  /// @param The new pin position.
  //-----------------------------------------------------------------------------------------------------
  void setPinPosition(const glm::vec3 &_pin);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief The position that the particle is pinned to.
  //-----------------------------------------------------------------------------------------------------
  glm::vec3 m_pin;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The index of the pinned particle.
  //-----------------------------------------------------------------------------------------------------
  size_t m_p;
};

}

#endif // PINCONSTRAINT_H

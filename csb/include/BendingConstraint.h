#ifndef BENDINGCONSTRAINT_H
#define BENDINGCONSTRAINT_H

#include "PositionConstraint.h"
#include <array>

namespace csb
{

class BendingConstraint : public PositionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  BendingConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor.
  /// @param _p1 is the first particle index that this constraint will be projected onto.
  /// @param _p2 is the second particle index that this constraint will be projected onto.
  /// @param _p3 is the third particle index that this constraint will be projected onto.
  /// @param _rest is distance from _p3 to the centroid of all 3 particles that satisfies the constraint.
  /// @param _stiffness is the aggressiveness of the constraint.
  /// @param _particles is the particle array that this constraint will affect, required as we need to,
  /// calculate the weights from their inverse masses
  //-----------------------------------------------------------------------------------------------------
  BendingConstraint(
      const size_t _p1,
      const size_t _p2,
      const size_t _p3,
      const float _rest,
      const float _stiffness,
      const std::vector<Particle>&_particles
      );
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  BendingConstraint(const BendingConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  BendingConstraint& operator=(const BendingConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  BendingConstraint(BendingConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  BendingConstraint& operator=(BendingConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~BendingConstraint() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles. Attempts to return them to their original,
  /// aligment.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(std::vector<Particle> &io_particles) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual PositionConstraint* clone() const override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the particle index for the requested particle.
  /// @param _index is the particle index to retrieve from the internal particle index array.
  /// @return A an index into a meshes particles.
  //-----------------------------------------------------------------------------------------------------
  size_t getParticleIndex(const unsigned short _index) const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the particle weight in this constraint for the requested particle.
  /// @param _index is the index of the particle whos weight we should retrieve.
  /// @return The weight of the requested particle.
  //-----------------------------------------------------------------------------------------------------
  float  getParticleWeight(const unsigned short _index) const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the rest distance of the 3rd particle from the centroid of all 3 particles.
  /// @return The rest distance of m_p[3] from the centroid.
  //-----------------------------------------------------------------------------------------------------
  float  getRest() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets the stiffness of the bending constraint.
  /// @return The stiffness, how forcefully the constraint is enforced.
  //-----------------------------------------------------------------------------------------------------
  float  getStiffness() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the particle index in the internal particle index array.
  /// @param _index is index in the internal array to set.
  /// @param _p is the new value for the particle index.
  //-----------------------------------------------------------------------------------------------------
  void setParticleIndex(const unsigned short _index, const size_t &_p);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the particle weight in this constraint for the requested particle.
  /// @param _index is the index of the particle whos weight we should set.
  /// @param _weight is the new weight of the requested particle.
  //-----------------------------------------------------------------------------------------------------
  void setParticleWeight(const unsigned short _index, const float _weight);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the rest distance of the 3rd particle from the centroid of all 3 particles.
  /// @param _rest is the new rest distance.
  //-----------------------------------------------------------------------------------------------------
  void setRest(const float _rest);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the stiffness of the bending constraint.
  /// @param _stiffness is the new stiffness, how forcefully the constraint is enforced.
  //-----------------------------------------------------------------------------------------------------
  void setStiffness(const float _stiffness);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief The particle indices that this constraint applies to.
  //-----------------------------------------------------------------------------------------------------
  std::array<size_t, 3> m_p {{0ul, 0ul, 0ul}};
  //-----------------------------------------------------------------------------------------------------
  /// @brief The weights for each particle which determine how the load of satisfying the constrant is,
  /// distributed.
  //-----------------------------------------------------------------------------------------------------
  std::array<float, 3>  m_w {{0.f, 0.f, 0.f}};
  //-----------------------------------------------------------------------------------------------------
  /// @brief The rest distance, of the 3rd particle from the centroid of all 3 particles.
  //-----------------------------------------------------------------------------------------------------
  float m_rest = 0.f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief How forcefully the constraint is enforced.
  //-----------------------------------------------------------------------------------------------------
  float m_stiffness = 0.025f;
};

}

#endif // BENDINGCONSTRAINT_H

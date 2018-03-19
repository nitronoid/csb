#ifndef STATICCOLLISIONCONSTRAINT_H
#define STATICCOLLISIONCONSTRAINT_H

#include <vector>
#include <QOpenGLFunctions>
#include "Particle.h"
#include "SpatialHash.h"

namespace csb
{

class StaticCollisionConstraint
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint(const StaticCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint& operator=(const StaticCollisionConstraint&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint(StaticCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  StaticCollisionConstraint& operator=(StaticCollisionConstraint&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~StaticCollisionConstraint();
  //-----------------------------------------------------------------------------------------------------
  /// @brief This projects the constraint onto the particles.
  /// @param io_particles is a reference to the particles that this constraint is to be projected on to.
  /// @param _spatialHash is a reference to the solver's hash table which can be used to make the collision,
  /// test more efficient.
  //-----------------------------------------------------------------------------------------------------
  virtual void project(std::vector<Particle> &io_particles, const SpatialHash::SpatialHashTable &_spatialHash) = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is used by classes that contain constraints to implement copy constructors.
  /// @return A raw pointer to a newly allocated clone of the derived class.
  //-----------------------------------------------------------------------------------------------------
  virtual StaticCollisionConstraint* clone() const = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief This init function will be called post setting the cell and hash table sizes, inside of the,
  /// solver, so any calculations involving them for spatial hashing should take place here.
  //-----------------------------------------------------------------------------------------------------
  virtual void init() = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the member that tracks the hash table size, used for spatial hashing.
  /// @param _newSize is the size of the hash table.
  //-----------------------------------------------------------------------------------------------------
  void setHashTableSize(const size_t &_newSize);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the member that tracks the spatial cell size, used for spatial hashing.
  /// @param _newSize is the cell size.
  //-----------------------------------------------------------------------------------------------------
  void setCellSize(const float _newSize);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Sets the member that tracks the spatial cell offset, used for spatial hashing.
  /// @param _newOffset is the cell offset.
  //-----------------------------------------------------------------------------------------------------
  void setCellOffset(const float _newOffset);

protected:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Tracks hash table size, used for spatial hashing.
  //-----------------------------------------------------------------------------------------------------
  size_t m_hashTableSize = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Tracks the spatial cell size, used for spatial hashing.
  //-----------------------------------------------------------------------------------------------------
  float m_cellSize = 0.f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The offset to be added to particle positions before hashing.
  //-----------------------------------------------------------------------------------------------------
  float m_cellOffset = 0.0f;

};

}

#endif // STATICCOLLISIONCONSTRAINT_H

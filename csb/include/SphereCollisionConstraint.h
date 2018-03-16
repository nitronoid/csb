#ifndef SPHERECOLLISIONCONSTRAINT_H
#define SPHERECOLLISIONCONSTRAINT_H

#include "StaticCollisionConstraint.h"


namespace csb
{

class SphereCollisionConstraint : public StaticCollisionConstraint
{
public:
  SphereCollisionConstraint(const glm::vec3 &_centre, const float _radius, const float _cellSize, const size_t& _hashTableSize);
  SphereCollisionConstraint(const SphereCollisionConstraint&) = default;
  SphereCollisionConstraint& operator=(const SphereCollisionConstraint&) = default;
  SphereCollisionConstraint(SphereCollisionConstraint&&) = default;
  SphereCollisionConstraint& operator=(SphereCollisionConstraint&&) = default;
  virtual ~SphereCollisionConstraint() override = default;
  virtual void project(std::vector<Particle> &_positions, const std::vector<std::vector<std::pair<GLushort, GLushort>>>& _spatialHash) override;
  virtual StaticCollisionConstraint* clone() const override;

private:

  std::vector<size_t> m_cells;
  glm::vec3 m_centre;
  float m_radius;
};

}

#endif // SPHERECOLLISIONCONSTRAINT_H

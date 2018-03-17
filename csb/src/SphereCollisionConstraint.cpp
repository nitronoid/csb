#include "SphereCollisionConstraint.h"
#include "SpatialHash.h"
#include "common.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/fast_square_root.hpp"
#undef GLM_ENABLE_EXPERIMENTAL


csb::SphereCollisionConstraint::SphereCollisionConstraint(const glm::vec3 &_centre, const float _radius, const float _cellSize, const size_t&_hashTableSize) :
  m_centre(_centre),
  m_radius(_radius)
{
  const auto bbMin = _centre - glm::vec3(_radius);
  const auto bbMax = _centre + glm::vec3(_radius);

  const auto min = SpatialHash::calcCell(bbMin, _cellSize);
  const auto max = SpatialHash::calcCell(bbMax, _cellSize);

  // hash all cells within the bounding box of this sphere
  for (int x = min.x; x <= max.x; ++x)
    for (int y = min.y; y <= max.y; ++y)
      for (int z = min.z; z <= max.z; ++z)
      {
        m_cells.push_back(SpatialHash::hashCell({x,y,z}, _hashTableSize));
      }
}

void csb::SphereCollisionConstraint::project(std::vector<Particle> &_particles, const std::vector<std::vector<std::pair<GLushort, GLushort>>> &_spatialHash)
{
  // Get the particles within the spheres bounding box
  std::vector<size_t> intersectingParticles;
  for (const auto& cell : m_cells)
  {
    const auto& contained = _spatialHash[cell];
    intersectingParticles.reserve(intersectingParticles.size() + contained.size());
    for (const auto& particleId : contained)
      intersectingParticles.push_back(particleId.second);
  }


  for (const auto& p : intersectingParticles)
  {
    const auto disp = *_particles[p].m_pos - m_centre;
    const auto distanceFromCentre = glm::fastLength(disp);
    if (distanceFromCentre <= m_radius)
    {
      *_particles[p].m_pos = m_centre + glm::fastNormalize(disp) * m_radius;
    }
  }
}

csb::StaticCollisionConstraint* csb::SphereCollisionConstraint::clone() const
{
  return new SphereCollisionConstraint(*this);
}

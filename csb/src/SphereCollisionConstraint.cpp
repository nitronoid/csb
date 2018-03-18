#include "SphereCollisionConstraint.h"
#include "SpatialHash.h"
#include "common.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/fast_square_root.hpp"
#undef GLM_ENABLE_EXPERIMENTAL


csb::SphereCollisionConstraint::SphereCollisionConstraint(const glm::vec3 &_centre, const float _radius) :
  m_centre(_centre),
  m_radius(_radius)
{}

void csb::SphereCollisionConstraint::init()
{
  updateBoundingBoxCells();
}


void csb::SphereCollisionConstraint::updateBoundingBoxCells()
{
  const auto bbMin = m_centre - glm::vec3(m_radius);
  const auto bbMax = m_centre + glm::vec3(m_radius);

  const auto min = SpatialHash::calcCell(bbMin, m_cellSize);
  const auto max = SpatialHash::calcCell(bbMax, m_cellSize);

  m_cells.clear();
  m_cells.reserve(static_cast<size_t>((max.x - min.x) * (max.y - min.y) * (max.z - min.z)));
  // hash all cells within the bounding box of this sphere
  for (int x = min.x; x <= max.x; ++x)
    for (int y = min.y; y <= max.y; ++y)
      for (int z = min.z; z <= max.z; ++z)
      {
        m_cells.push_back(SpatialHash::hashCell({x,y,z}, m_hashTableSize));
      }
}

const std::vector<size_t> csb::SphereCollisionConstraint::cells() const noexcept
{
  return m_cells;
}

glm::vec3 csb::SphereCollisionConstraint::getCentre() const noexcept
{
  return m_centre;
}

float csb::SphereCollisionConstraint::getRadius() const noexcept
{
  return m_radius;
}

void csb::SphereCollisionConstraint::setCentre(const glm::vec3 &_centre)
{
  m_centre = _centre;
}

void csb::SphereCollisionConstraint::setRadius(const float _radius)
{
  m_radius = _radius;
}

void csb::SphereCollisionConstraint::project(std::vector<Particle> &io_particles, const SpatialHash::SpatialHashTable &_spatialHash)
{
  // Get the particles within the spheres bounding box
  std::vector<size_t> intersectingParticles;
  for (const auto& cell : m_cells)
  {
    const auto& contained = _spatialHash.m_hashTable[cell];
    intersectingParticles.reserve(intersectingParticles.size() + contained.size());
    for (const auto& particleId : contained)
      intersectingParticles.push_back(particleId.second);
  }


  for (const auto& p : intersectingParticles)
  {
    const auto disp = *io_particles[p].m_pos - m_centre;
    const auto distanceFromCentre = glm::fastLength(disp);
    if (distanceFromCentre <= m_radius)
    {
      *io_particles[p].m_pos = m_centre + glm::fastNormalize(disp) * m_radius;
    }
  }
}

csb::StaticCollisionConstraint* csb::SphereCollisionConstraint::clone() const
{
  return new SphereCollisionConstraint(*this);
}

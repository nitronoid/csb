#include "SelfCollisionSpheresConstraint.h"
#include "SpatialHash.h"
#include "gtx/norm.hpp"
#include "gtx/fast_square_root.hpp"


//----------------------------------------------------------------------------------------------------------------------------
void csb::SelfCollisionSpheresConstraint::project(
    const size_t &_meshIndex,
    std::vector<std::shared_ptr<SimulatedMesh> > &_meshes,
    const SpatialHash::SpatialHashTable &_spatialHash
    )
{
  auto& mesh = *_meshes[_meshIndex];
  const auto& adjacency = mesh.getAdjacencyInfo();
  auto& particles = getParticles(mesh);
  const auto size = mesh.getNVerts();
  for (GLushort i = 0; i < size; ++i)
  {
    auto& P = particles[i];
    // Build a list of ignored particles, consiting of itself and it's first ring neighbours.
    auto ignored = adjacency[i];
    ignored.push_back(i);
    std::sort(ignored.begin(), ignored.end());

    const auto& hashTable = _spatialHash.m_hashTable;
    // Get all of the particles in this cell
    auto considered = hashTable[SpatialHash::hashParticle(*P.m_pos, hashTable.size(), m_sphereDiameter, _spatialHash.m_cellOffset)];
    std::sort(considered.begin(), considered.end());


    // Scope the using declaration
    {
      // I think this is more readable, we erase the ignored particles from the list of considered ones
      using namespace std;
      considered.erase(
            remove_if(begin(considered), end(considered), [&ignored, &_meshIndex](const auto x)
      {
        return (x.first == _meshIndex) && binary_search(begin(ignored), end(ignored),x.second);
      }
      ), end(considered));
    }

    // We'll accumulate all the collision responses here
    glm::vec3 offset(0.f);
    int intersectionCount = 0;

    for (const auto& pid : considered)
    {
      // Get the particle we are checking against
      auto& otherParticles = getParticles(*_meshes[pid.first]);
      const auto& Q = otherParticles[pid.second];
      // Get the displacement vector and squared distance between the particles
      const auto disp = *P.m_pos - *Q.m_pos;
      const auto dist = glm::length2(disp);

      // By setting the distance to be larger than the distance between particles
      // we should cover the cloth surface, however we can't set them too big,
      // otherwise conflicts with neighbours will occur and we'll see flickering.
      // We check the squared distances to avoid expensive length calculations.
      if (dist < m_sphereDiameter * m_sphereDiameter)
      {
        // Get the correction vector (2R - D)
        const auto move = (m_sphereDiameter - glm::fastSqrt(dist));
        offset += (glm::fastNormalize(disp) * move);
        ++intersectionCount;
      }
    }

    // If an intersection took place
    if (intersectionCount)
    {
      // Set a lower bound for the offset to reduce flickering
      offset *= glm::step(0.001f, offset);
      // Average the correction
      (*P.m_pos) += offset/static_cast<float>(intersectionCount);
      // zero the velocity
      P.m_prevPos = *P.m_pos;
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------------
csb::ContinuousCollisionConstraint* csb::SelfCollisionSpheresConstraint::clone() const
{
  return new SelfCollisionSpheresConstraint(*this);
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::SelfCollisionSpheresConstraint::setSphereDiameter(const float _radius)
{
  m_sphereDiameter = _radius;
}
//----------------------------------------------------------------------------------------------------------------------------
float csb::SelfCollisionSpheresConstraint::getSphereDiameter() const noexcept
{
  return m_sphereDiameter;
}
//----------------------------------------------------------------------------------------------------------------------------

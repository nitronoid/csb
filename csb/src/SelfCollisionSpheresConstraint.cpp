#include "SelfCollisionSpheresConstraint.h"
#include "SpatialHash.h"
#include "gtx/norm.hpp"
#include "gtx/fast_square_root.hpp"


void csb::SelfCollisionSpheresConstraint::project(const size_t &_meshIndex,
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
    auto ignored = adjacency[i];
    ignored.push_back(i);
    std::sort(ignored.begin(), ignored.end());

    auto considered = _spatialHash.m_hashTable[SpatialHash::hashParticle(*P.m_pos, _spatialHash.m_hashTable.size(), m_sphereRadius)];
    std::sort(considered.begin(), considered.end());


    // Scope the using declaration
    {
      // I think this is more readable
      using namespace std;
      considered.erase(
            remove_if(begin(considered), end(considered),
                      [&ignored, &_meshIndex](const auto x)
      {
        return (x.first == _meshIndex) && binary_search(begin(ignored), end(ignored),x.second);
      }),
            end(considered)
            );
    }

    glm::vec3 offset(0.f);
    int count = 0;

    for (const auto& pid : considered)
    {
      auto& otherParticles = getParticles(*_meshes[pid.first]);
      const auto& Q = otherParticles[pid.second];
      const auto disp = *P.m_pos - *Q.m_pos;
      const auto dist = glm::length2(disp);

      // By setting the distance to be larger than the distance between particles
      // we should cover the cloth surface, however we can't set them too big,
      // otherwise conflicts with neighbours will occur and we'll see flickering
      auto radius_sqr = (m_sphereRadius);
      radius_sqr *= radius_sqr;
      if (dist < radius_sqr)
      {
        const auto move = (glm::fastSqrt(radius_sqr) - glm::fastSqrt(dist)) * 0.5f;
        offset += (glm::fastNormalize(disp) * move);
        ++count;
      }
    }

    if (count)
    {
      // Set a lower bound for the offset to reduce flickering
      offset *= glm::step(0.001f, offset);
      (*P.m_pos) += offset/static_cast<float>(count);
      // zero the velocity
      P.m_prevPos = *P.m_pos;
    }
  }
}


csb::ContinuousCollisionConstraint* csb::SelfCollisionSpheresConstraint::clone() const
{
  return new SelfCollisionSpheresConstraint(*this);
}

void csb::SelfCollisionSpheresConstraint::setSphereRadius(const float _radius)
{
  m_sphereRadius = _radius;
}

float csb::SelfCollisionSpheresConstraint::getSphereRadius() const noexcept
{
  return m_sphereRadius;
}


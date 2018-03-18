#include "SelfCollisionRaysConstraint.h"
#include "SpatialHash.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/fast_square_root.hpp"
#include "gtx/norm.hpp"
#include "gtx/normal.hpp"
#include "gtx/intersect.hpp"
#undef GLM_ENABLE_EXPERIMENTAL


void csb::SelfCollisionRaysConstraint::project(const size_t &_meshIndex,
    std::vector<std::shared_ptr<SimulatedMesh> > &_meshes,
    const SpatialHash::SpatialHashTable &_spatialHash
    )
{
  const auto hashOffset = _spatialHash.m_triHashOffset[_meshIndex];
  auto& mesh = *_meshes[_meshIndex];
  const auto& indices = mesh.getIndices();
  auto& particles = getParticles(mesh);
  const auto size = mesh.getNIndices() / 3;
  // Loop over all faces
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& T0 = *particles[indices[index]].m_pos;
    const auto& T1 = *particles[indices[index + 1]].m_pos;
    const auto& T2 = *particles[indices[index + 2]].m_pos;
    const auto TNorm = glm::triangleNormal(T0, T1, T2);

    // Loop over all hashed cells for this face
    for (const auto& hashCell : _spatialHash.m_triangleBoundingBoxHash[i + hashOffset])
    {
      // Loop over all particles in the cell
      for (const auto& meshPid : _spatialHash.m_hashTable[hashCell])
      {
        const auto meshId = meshPid.first;
        const auto pid = meshPid.second;
        // skip the particles in this meshes triangle
        if (meshId == _meshIndex && ((pid == indices[index]) || (pid == indices[index + 1]) || (pid == indices[index + 2])))
          continue;
        auto& otherParticles = getParticles(*_meshes[meshId]);
        const auto& particle = otherParticles[pid];
        const auto dir = *particle.m_pos - particle.m_prevPos;
        const auto& L0 = *particle.m_pos;
        const auto& L1 = L0 + dir;

        // Use these vectors to determine whether the line is entirely on one side of the tri
        const auto distStart = glm::dot(T0 - L0, TNorm);
        const auto distEnd = glm::dot(T0 - L1, TNorm);

        glm::vec3 bary;
        // Check not same side of triangle, and an intersection is present
        if (glm::intersectLineTriangle(L0, dir, T0, T1, T2, bary) && (distStart * distEnd < 0.0f))
        {
          // We swap the past and current positions to reverse velocity giving a slight bounce to the cloth
//          std::swap(*particles[indices[index]].m_pos, particles[indices[index]].m_prevPos);
//          std::swap(*particles[indices[index + 1]].m_pos, particles[indices[index + 1]].m_prevPos);
//          std::swap(*particles[indices[index + 2]].m_pos, particles[indices[index + 2]].m_prevPos);
//          // this one belongs to the other mesh
//          std::swap(*otherMesh->m_particles[pid].m_pos, otherMesh->m_particles[pid].m_prevPos);
          particles[indices[index]].m_prevPos = *particles[indices[index]].m_pos;
          particles[indices[index + 1]].m_prevPos = *particles[indices[index + 1]].m_pos;
          particles[indices[index + 2]].m_prevPos = *particles[indices[index + 2]].m_pos;
          otherParticles[pid].m_prevPos = *otherParticles[pid].m_pos;
        }
      }
    }
  }
}


csb::ContinuousCollisionConstraint* csb::SelfCollisionRaysConstraint::clone() const
{
  return new SelfCollisionRaysConstraint(*this);
}

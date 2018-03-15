#include "CSBsolver.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/fast_square_root.hpp"
#include "gtx/norm.hpp"
#include "gtx/normal.hpp"
#include "gtx/intersect.hpp"
//#undef GLM_ENABLE_EXPERIMENTAL
#include <random>
#include <algorithm>

std::vector<GLushort> CSBsolver::getConnectedVertices(CSBmesh* _meshRef, const GLushort _vert)
{
  return _meshRef->getAdjacencyInfo()[_vert];
}

glm::ivec3 CSBsolver::calcCell(const glm::vec3& _coord) const
{
  // cellsize is equal to the average edge length for max performance
  return glm::ivec3(
        static_cast<int>(glm::floor(_coord.x / m_avgEdgeLength)),
        static_cast<int>(glm::floor(_coord.y / m_avgEdgeLength)),
        static_cast<int>(glm::floor(_coord.z / m_avgEdgeLength))
        );
}

size_t CSBsolver::hashCell (const glm::ivec3& _cell) const
{
  static constexpr auto posMod = [](const auto _x, const auto _m)
  {
    return ((static_cast<size_t>(_x) % _m) + _m) % _m;
  };

  static constexpr int primes[] = {73856093, 19349663, 83492791};
  return posMod((_cell.x * primes[0]) ^ (_cell.y * primes[1]) ^ (_cell.z * primes[2]), m_hashTable.size());
}

size_t CSBsolver::hashParticle (const glm::vec3& _coord) const
{
  return hashCell(calcCell(_coord));
}

void CSBsolver::hashVerts(const size_t &_meshIndex)
{
  const auto mesh = m_referencedMeshes[_meshIndex];
  const auto size = mesh->getNVerts();
  for (GLushort i = 0; i < size; ++i)
  {
    m_hashTable[hashParticle(mesh->m_particles[i].m_pos)].emplace_back(_meshIndex, i);
  }
}

void CSBsolver::hashTris(const size_t &_meshIndex)
{
  const auto mesh = m_referencedMeshes[_meshIndex];
  const auto hashOffset = m_triHashOffset[_meshIndex];
  const auto size = mesh->getNIndices() / 3;
  const auto& indices = mesh->getIndices();
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& p1 = mesh->m_particles[indices[index]];
    const auto& p2 = mesh->m_particles[indices[index + 1]];
    const auto& p3 = mesh->m_particles[indices[index + 2]];

    const auto min = calcCell(glm::min(glm::min(p1.m_pos, p2.m_pos), p3.m_pos));
    const auto max = calcCell(glm::max(glm::max(p1.m_pos, p2.m_pos), p3.m_pos));

    // hash all cells within the bounding box of this triangle
    for (int x = min.x; x <= max.x; ++x)
      for (int y = min.y; y <= max.y; ++y)
        for (int z = min.z; z <= max.z; ++z)
        {
          m_triangleVertHash[i + hashOffset].push_back(hashCell({x,y,z}));
        }
  }
}

void CSBsolver::resolveContinuousCollision_spheres(const size_t &_meshIndex)
{
  const auto mesh = m_referencedMeshes[_meshIndex];
  const auto size = mesh->getNVerts();
  for (GLushort i = 0; i < size; ++i)
  {
    auto& P = mesh->m_particles[i];
    auto ignored = getConnectedVertices(mesh, i);
    ignored.push_back(i);
    std::sort(ignored.begin(), ignored.end());

    auto considered = m_hashTable[hashParticle(P.m_pos)];
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
      const auto& Q = m_referencedMeshes[pid.first]->m_particles[pid.second];
      const auto disp = P.m_pos - Q.m_pos;
      const auto dist = glm::length2(disp);

      // By setting the distance to be larger than the distance between particles
      // we should cover the cloth surface, however we can't set them too big,
      // otherwise conflicts with neighbours will occur and we'll see flickering
      auto radius_sqr = (m_shortestEdgeDist * 1.25f);
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
      P.m_pos += offset/static_cast<float>(count);
      // zero the velocity
      P.m_prevPos = P.m_pos;
    }
  }
}

void CSBsolver::resolveContinuousCollision_rays(const size_t &_meshIndex)
{
  const auto hashOffset = m_triHashOffset[_meshIndex];
  const auto mesh = m_referencedMeshes[_meshIndex];
  const auto& indices = mesh->getIndices();
  auto& particles = mesh->m_particles;
  const auto size = mesh->getNIndices() / 3;
  // Loop over all faces
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& T0 = particles[indices[index]].m_pos;
    const auto& T1 = particles[indices[index + 1]].m_pos;
    const auto& T2 = particles[indices[index + 2]].m_pos;
    const auto TNorm = glm::triangleNormal(T0, T1, T2);

    // Loop over all hashed cells for this face
    for (const auto& hashCell : m_triangleVertHash[i + hashOffset])
    {
      // Loop over all particles in the cell
      for (const auto& meshPid : m_hashTable[hashCell])
      {
        const auto meshId = meshPid.first;
        const auto pid = meshPid.second;
        // skip the particles in this meshes triangle
        if (meshId == _meshIndex && ((pid == indices[index]) || (pid == indices[index + 1]) || (pid == indices[index + 2])))
          continue;
        auto& otherMesh = m_referencedMeshes[meshId];
        const auto& particle = otherMesh->m_particles[pid];
        const auto& L0 = particle.m_prevPos;
        const auto& L1 = particle.m_pos;
        const auto dir = L1 - L0;

        glm::vec3 bary;

        const auto distStart = glm::dot(T0 - L0, TNorm);
        const auto distEnd = glm::dot(T0 - L1, TNorm);

        // Check not same side of triangle, and an intersection is present
        if (glm::intersectLineTriangle(L0, dir, T0, T1, T2, bary) && (distStart * distEnd < 0.0f))
        {
          // We swap the past and current positions to reverse velocity giving a slight bounce to the cloth
          std::swap(particles[indices[index]].m_pos, particles[indices[index]].m_prevPos);
          std::swap(particles[indices[index + 1]].m_pos, particles[indices[index + 1]].m_prevPos);
          std::swap(particles[indices[index + 2]].m_pos, particles[indices[index + 2]].m_prevPos);
          // this one belongs to the other mesh
          std::swap(otherMesh->m_particles[pid].m_pos, otherMesh->m_particles[pid].m_prevPos);
        }
      }
    }
  }
}


void CSBsolver::addTriangleMesh(CSBmesh& _mesh)
{
  // Store a reference pointer to the mesh
  m_referencedMeshes.push_back(&_mesh);
  m_vertHashOffset.push_back(m_numParticles);
  m_triHashOffset.push_back(m_numTris);
  // Update our stored counters
  m_numParticles += _mesh.getNVerts();
  m_numTris += _mesh.getNIndices() / 3;
  m_totalEdgeLength += _mesh.getTotalEdgeLength();
  m_numEdges += _mesh.getNEdges();
  m_avgEdgeLength = (m_totalEdgeLength + _mesh.getTotalEdgeLength()) / m_numEdges;
  m_shortestEdgeDist = std::min(m_shortestEdgeDist, _mesh.getShortestEdgeLength());

  // Calculate optimal hash table size
  m_triangleVertHash.resize(m_numTris);
  const size_t multiple = static_cast<size_t>(pow10(floor(log10(m_numParticles))));
  const auto hashTableSize = ((m_numParticles + multiple - 1) / multiple) * multiple - 1;
  m_hashTable.resize(hashTableSize);

}

void CSBsolver::update(const float _time)
{
  for (auto mesh : m_referencedMeshes)
    mesh->projectConstraints();


  for (auto& hash : m_triangleVertHash) hash.clear();
  for (auto& cell : m_hashTable) cell.clear();

  const auto numMeshes = m_referencedMeshes.size();
  for (size_t i = 0; i < numMeshes; ++i)
  {
    hashVerts(i);
    hashTris(i);
  }

  for (size_t i = 0; i < numMeshes; ++i)
  {
    resolveContinuousCollision_rays(i);
    resolveContinuousCollision_spheres(i);
  }

  const auto force = glm::vec3(0.f, -10.f, 0.f);
  static constexpr auto damping = 0.9f;

  for (auto mesh : m_referencedMeshes)
  {
    for (auto& particle : mesh->m_particles)
    {
      const auto vel = (particle.m_pos - particle.m_prevPos ) * damping;
      const auto newPos = particle.m_pos + vel + (particle.m_invMass * force * _time * _time);
      particle.m_prevPos = particle.m_pos;
      particle.m_pos = newPos;
    }
  }

}

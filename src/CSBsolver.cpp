#include "CSBsolver.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/fast_square_root.hpp"
#include "gtx/norm.hpp"
#include "gtx/normal.hpp"
#include "gtx/intersect.hpp"
//#undef GLM_ENABLE_EXPERIMENTAL
#include <random>
#include <algorithm>

std::unordered_set<CSBsolver::EdgePair> CSBsolver::getEdges(Mesh *_meshRef)
{
  std::unordered_set<EdgePair> edgeSet;
  const auto& indices = _meshRef->getIndices();
  auto numEdges = _meshRef->getNVerts() + (indices.size() / 3) - 2;
  edgeSet.reserve(numEdges);

  const auto last = indices.size() - 2;
  for (size_t i = 0; i < last; i+=3)
  {
    const auto p1 = indices[i];
    const auto p2 = indices[i + 1];
    const auto p3 = indices[i + 2];
    edgeSet.insert({p1, p2});
    edgeSet.insert({p2, p3});
    edgeSet.insert({p3, p1});
  }
  return edgeSet;
}


std::vector<GLushort> CSBsolver::getConnectedVertices(Mesh *_meshRef, const GLushort _vert)
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

void CSBsolver::hashVerts()
{
  for (auto& cell : m_hashTable) cell.clear();
  for (GLushort i = 0; i < m_particles.size(); ++i)
  {
    m_hashTable[hashParticle(m_particles[i].m_pos)].push_back(i);
  }
}

void CSBsolver::hashTris()
{
  for (auto& hash : m_triangleVertHash) hash.clear();
  const auto size = m_triangleVertHash.size();
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& p1 = m_particles[m_indices[index]];
    const auto& p2 = m_particles[m_indices[index + 1]];
    const auto& p3 = m_particles[m_indices[index + 2]];

    const auto min = calcCell(glm::min(glm::min(p1.m_pos, p2.m_pos), p3.m_pos));
    const auto max = calcCell(glm::max(glm::max(p1.m_pos, p2.m_pos), p3.m_pos));

    // hash all cells within the bounding box of this triangle
    for (int x = min.x; x <= max.x; ++x)
      for (int y = min.y; y <= max.y; ++y)
        for (int z = min.z; z <= max.z; ++z)
        {
          m_triangleVertHash[i].push_back(hashCell({x,y,z}));
        }
  }
}

void CSBsolver::resolveSelfCollision_spheres()
{
  const auto size = m_particles.size();
  for (GLushort i = 0; i < size; ++i)
  {
    auto& P = m_particles[i];

    //-------------------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------TESTING ONLY!!!!!!!!!!!!!!------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------

    auto ignored = getConnectedVertices(m_referencedMeshes[0], i);
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
                      [&ignored](const auto x) { return binary_search(begin(ignored), end(ignored),x); }),
            end(considered)
            );
    }


    glm::vec3 offset(0.f);
    int count = 0;
    for (const auto& pid : considered)
    {
      const auto& Q = m_particles[pid];
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

void CSBsolver::resolveSelfCollision_rays()
{
  const auto size = m_triangleVertHash.size();
  //     Loop over all faces
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& T0 = m_particles[m_indices[index]].m_pos;
    const auto& T1 = m_particles[m_indices[index + 1]].m_pos;
    const auto& T2 = m_particles[m_indices[index + 2]].m_pos;
    const auto TNorm = glm::triangleNormal(T0, T1, T2);

    // Loop over all hashed cells for this face
    for (const auto& hash : m_triangleVertHash[i])
    {
      // Loop over all particles in the cell
      for (const auto& pid : m_hashTable[hash])
      {
        // skip the particles in this face
        if ((pid == m_indices[index]) || (pid == m_indices[index + 1]) || (pid == m_indices[index + 2]))
          continue;
        const auto& particle = m_particles[pid];
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
          std::swap(m_particles[m_indices[index]].m_pos, m_particles[m_indices[index]].m_prevPos);
          std::swap(m_particles[m_indices[index + 1]].m_pos, m_particles[m_indices[index + 1]].m_prevPos);
          std::swap(m_particles[m_indices[index + 2]].m_pos, m_particles[m_indices[index + 2]].m_prevPos);
          std::swap(m_particles[pid].m_pos, m_particles[pid].m_prevPos);
        }
      }
    }
  }
}


void CSBsolver::addTriangleMesh(Mesh &_mesh)
{
  m_referencedMeshes.push_back(&_mesh);
  m_triangleVertHash.resize(m_indices.size() / 3);

  const auto& meshIndices = _mesh.getIndices();
  m_indices.reserve(m_indices.size() + distance(meshIndices.begin(), meshIndices.end()));
  m_indices.insert(m_indices.end(), meshIndices.begin(), meshIndices.end());

  // Calculate optimal hash table size
  const auto numVerts = _mesh.getNVerts();
  const size_t multiple = static_cast<size_t>(pow10(floor(log10(numVerts))));
  const auto hashTableSize = ((numVerts + multiple - 1) / multiple) * multiple - 1;
  m_hashTable.resize(hashTableSize);
  m_particles.reserve(numVerts);

  auto& meshVerts = _mesh.getVertices();
  for (auto& vert : meshVerts)
    m_particles.emplace_back(vert, 1.f);

  m_particles[0].m_invMass = 0.f;
  m_particles[m_particles.size() - 1].m_invMass = 0.f;

  auto edgeSet = getEdges(&_mesh);
  float totalEdgeDist = 0.0f;
  const auto& firstEdge = edgeSet.begin()->p;
  m_shortestEdgeDist = glm::fastDistance(meshVerts[m_indices[firstEdge.first]], meshVerts[m_indices[firstEdge.second]]);
  for (const auto & edge : edgeSet)
  {
    const auto p1 = edge.p.first;
    const auto p2 = edge.p.second;
    const auto distance = glm::fastDistance(meshVerts[p1], meshVerts[p2]);
    m_shortestEdgeDist = std::min(m_shortestEdgeDist, distance);
    totalEdgeDist += distance;
    m_constraints.emplace_back(new DistanceConstraint(p1, p2, distance));
  }

  m_avgEdgeLength = totalEdgeDist / edgeSet.size();

  std::unordered_set<EdgePair> connections;
  for (GLushort v = 0; v < numVerts; ++v)
  {
    auto neighbours = getConnectedVertices(&_mesh, v);
    for (const auto vi : neighbours)
    {
      float bestCosTheta = 0.0f;
      auto bestV = vi;
      for (const auto vj : neighbours)
      {
        if (vj == vi) continue;
        auto a = meshVerts[vi] - meshVerts[v];
        auto b = meshVerts[vj] - meshVerts[v];
        auto cosTheta = glm::dot(a, b) / (glm::fastLength(a) * glm::fastLength(b));
        if (cosTheta < bestCosTheta)
        {
          bestCosTheta = cosTheta;
          bestV = vj;
        }
      }
      EdgePair connection {bestV, vi};
      if (!connections.count(connection))
      {
        connections.insert(connection);
        static constexpr float third = 1.0f / 3.0f;
        auto centre = third * (meshVerts[vi] + meshVerts[bestV] + meshVerts[v]);
        auto rest = glm::fastDistance(meshVerts[v], centre);
        m_constraints.emplace_back(new BendingConstraint(vi, bestV, v, rest, m_particles));
      }
    }
  }
}

void CSBsolver::update(const float _time)
{
  for (int i = 0; i < 30; ++i)
    for (auto& constraint : m_constraints)
    {
      constraint->project(m_particles);
    }

  hashVerts();
  hashTris();

  resolveSelfCollision_rays();

  resolveSelfCollision_spheres();

  const auto force = glm::vec3(0.f, -9.f, 0.f);
  const auto size = m_particles.size();
  static constexpr auto damping = 0.6f;


  for (size_t i = 0; i < size; ++i)
  {
    auto& particle = m_particles[i];
    const auto vel = (particle.m_pos - particle.m_prevPos ) * damping;
    const auto newPos = particle.m_pos + vel + (particle.m_invMass * force * _time * _time);
    particle.m_prevPos = particle.m_pos;
    particle.m_pos = newPos;
  }

}

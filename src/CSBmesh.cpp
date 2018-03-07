#include "CSBmesh.h"
#include "gtx/fast_square_root.hpp"

std::unordered_set<CSBmesh::EdgePair> CSBmesh::getEdges()
{
  std::unordered_set<EdgePair> edgeSet;
  auto numEdges = m_vertices.size() + (m_indices.size() / 3) - 2;
  edgeSet.reserve(numEdges);

  const auto last = m_indices.size() - 2;
  for (size_t i = 0; i < last; i+=3)
  {
    const auto p1 = m_indices[i];
    const auto p2 = m_indices[i + 1];
    const auto p3 = m_indices[i + 2];
    edgeSet.insert({p1, p2});
    edgeSet.insert({p2, p3});
    edgeSet.insert({p3, p1});
  }
  return edgeSet;
}


std::vector<GLushort> CSBmesh::getConnectedVertices(const GLushort _vert)
{
  std::unordered_set<GLushort> connected;

  const auto last = m_indices.size() - 2;
  for (size_t i = 0; i < last; i+=3)
  {
    std::unordered_set<GLushort> facePoints = {m_indices[i], m_indices[i + 1], m_indices[i + 2]};

    if (facePoints.count(_vert))
    {
      facePoints.erase(_vert);
      connected.insert(facePoints.begin(), facePoints.end());
    }
  }

  return std::vector<GLushort>{connected.begin(), connected.end()};
}

glm::ivec3 CSBmesh::calcCell(const glm::vec3& _coord) const
{
  static constexpr float cellsize = 0.8f;
  return glm::ivec3(
        static_cast<int>(glm::floor(_coord.x / cellsize)),
        static_cast<int>(glm::floor(_coord.y / cellsize)),
        static_cast<int>(glm::floor(_coord.z / cellsize))
        );
}

size_t CSBmesh::hashCell (const glm::ivec3& _cell) const
{
  static constexpr auto posMod = [](const int _x, const int _m)
  {
    return static_cast<size_t>(((_x % _m) + _m) % _m);
  };

  static constexpr int primes[] = {73856093, 19349663, 83492791};
  return posMod((_cell.x * primes[0]) ^ (_cell.y * primes[1]) ^ (_cell.z * primes[2]), 999);
}

size_t CSBmesh::hashPoint (const glm::vec3& _coord) const
{
  return hashCell(calcCell(_coord));
}

void CSBmesh::hashVerts()
{
  for (auto& cell : m_hashTable) cell.clear();
  for (GLushort i = 0; i < m_points.size(); ++i)
  {
    m_hashTable[hashPoint(m_points[i].m_pos)].push_back(i);
  }
}

void CSBmesh::hashTris()
{
  const auto size = m_triangleVertHash.size();
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& p1 = m_points[m_indices[index]];
    const auto& p2 = m_points[m_indices[index + 1]];
    const auto& p3 = m_points[m_indices[index + 2]];

    const auto min = calcCell(glm::min(glm::min(p1.m_pos, p2.m_pos), p3.m_pos));
    const auto max = calcCell(glm::max(glm::max(p1.m_pos, p2.m_pos), p3.m_pos));

    // hash all cells within the bounding box of this triangle
    for (int x = min.x; x < max.x; ++x)
      for (int y = min.y; y < max.y; ++y)
        for (int z = min.z; z < max.z; ++z)
        {
          m_triangleVertHash[i].push_back(hashCell({x,y,z}));
        }
  }
}

std::vector<PinConstraint> CSBmesh::generateCollisionConstraints()
{
  std::vector<PinConstraint> constraints;
  static constexpr auto fcomp = [](const float& _a, const float& _b)
  {
    return std::abs(_a - _b) < (0.001f * std::max(1.0f, std::max(std::abs(_a), std::abs(_b))));
  };
  const auto size = m_triangleVertHash.size();
  // Loop over all faces
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& T1 = m_points[m_indices[index]].m_pos;
    const auto& T2 = m_points[m_indices[index + 1]].m_pos;
    const auto& T3 = m_points[m_indices[index + 2]].m_pos;
    const auto norm = glm::normalize(glm::cross(T2 - T1, T3 - T1));

    // Loop over all hashed cells for this face
    for (const auto& hash : m_triangleVertHash[i])
    {
      // Loop over all points in the cell
      for (const auto& pid : m_hashTable[hash])
      {
        // skip the points in this face
        if ((pid == m_indices[index]) || (pid == m_indices[index] + 1) || (pid == m_indices[index + 2]))
          continue;
        const auto& point = m_points[pid];
        const auto& L1 = point.m_prevPos;
        const auto& L2 = point.m_pos;
        const auto DistStart = glm::dot(L1 - T1, norm);
        const auto DistEnd = glm::dot(L2 - T1, norm);

        auto intersection = L1 + (L2 - L1) * (-DistStart / (DistEnd - DistStart));

        if (!((DistStart * DistEnd >= 0.0f) // Check not same side of triangle
            || (fcomp(DistStart, DistEnd)) // Check not parallel to triangle
            || (glm::dot(glm::cross(norm, T2 - T1), intersection - T1) < 0.0f) // Check within triangle edges
            || (glm::dot(glm::cross(norm, T3 - T2), intersection - T2) < 0.0f)
            || (glm::dot(glm::cross(norm, T1 - T3), intersection - T1) < 0.0f)
              ))
        {
          // Add constraint here
          constraints.emplace_back(pid, intersection);
         // point.m_pos = point.m_prevPos;
        }
      }
    }
  }
  return constraints;
}

void CSBmesh::init()
{
  m_triangleVertHash.resize(m_indices.size() / 3);
  m_hashTable.resize(999);
  m_points.reserve(m_vertices.size());
  for (auto& vert : m_vertices)
    m_points.emplace_back(vert, 1.f);

  m_points[0].m_invMass = 0.f;
  m_points[m_points.size() - 1].m_invMass = 0.f;

  auto edgeSet = getEdges();
  for (const auto & edge : edgeSet)
  {
    auto p1 = edge.p.first;
    auto p2 = edge.p.second;
    float distance = glm::fastDistance(m_vertices[p1], m_vertices[p2]);
    m_constraints.emplace_back(new DistanceConstraint(p1, p2, distance));
  }

  const auto size = m_vertices.size();
  std::unordered_set<EdgePair> connections;
  for (GLushort v = 0; v < size; ++v)
  {
    auto neighbours = getConnectedVertices(v);
    for (const auto vi : neighbours)
    {
      float bestCosTheta = 0.0f;
      auto bestV = vi;
      for (const auto vj : neighbours)
      {
        if (vj == vi) continue;
        auto a = m_vertices[vi] - m_vertices[v];
        auto b = m_vertices[vj] - m_vertices[v];
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
        constexpr float third = 1.0f / 3.0f;
        auto centre = third * (m_vertices[vi] + m_vertices[bestV] + m_vertices[v]);
        auto rest = glm::fastDistance(m_vertices[v], centre);
        m_constraints.emplace_back(new BendingConstraint(vi, bestV, v, rest, m_points));
      }
    }
  }
}

void CSBmesh::update(const float _time)
{
  const auto gravity = glm::vec3(0.f,-4.95f,0.f);
  const auto size = m_points.size();
  for (size_t i = 0; i < size; ++i)
  {
    auto& point = m_points[i];
    glm::vec3 newPos = point.m_pos * 2.0f - point.m_prevPos + (point.m_invMass * gravity * _time * _time);
    point.m_prevPos = point.m_pos;
    point.m_pos = newPos;
  }
  hashVerts();
  hashTris();
  auto collisionConstraints = generateCollisionConstraints();
  for (auto& collisionConstraint : collisionConstraints)

    collisionConstraint.project(m_points);
  for (int i = 0; i < 5; ++i)
    for (auto& constraint : m_constraints)
    {
      constraint->project(m_points);
    }
}

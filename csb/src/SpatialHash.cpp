#include "SpatialHash.h"
#include "common.hpp"


//----------------------------------------------------------------------------------------------------------------------------
glm::ivec3 csb::SpatialHash::calcCell(const glm::vec3& _coord, const float _cellSize, const float _cellOffset)
{
  const auto coord = _coord + _cellOffset;
  // cellsize is equal to the average edge length for max performance
  return glm::ivec3(
        static_cast<int>(glm::floor(coord.x / _cellSize)),
        static_cast<int>(glm::floor(coord.y / _cellSize)),
        static_cast<int>(glm::floor(coord.z / _cellSize))
        );
}
//----------------------------------------------------------------------------------------------------------------------------
size_t csb::SpatialHash::hashCell(const glm::ivec3& _cell, const size_t &_tableSize)
{
  // C++ modulo doesn't always produce a positive output
  static constexpr auto posMod = [](const auto _x, const auto _m)
  {
    const auto m = static_cast<int>(_m);
    return static_cast<size_t>(((_x % m) + m) % m);
  };

  // From "Optimized Spatial Hashing for Collision Detection of Deformable Objects"
  static constexpr int primes[] = {73856093, 19349663, 83492791};
  return posMod((_cell.x * primes[0]) ^ (_cell.y * primes[1]) ^ (_cell.z * primes[2]), _tableSize);
}
//----------------------------------------------------------------------------------------------------------------------------
size_t csb::SpatialHash::hashParticle(const glm::vec3& _coord, const size_t &_tableSize, const float _cellSize, const float _cellOffset)
{
  // Convenience function
  return csb::SpatialHash::hashCell(calcCell(_coord, _cellSize, _cellOffset), _tableSize);
}
//----------------------------------------------------------------------------------------------------------------------------

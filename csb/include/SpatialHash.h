#ifndef SPATIALHASH_H
#define SPATIALHASH_H

#include "vec3.hpp"
#include "vector"
#include <QOpenGLFunctions>

namespace csb
{

namespace SpatialHash
{

struct SpatialHashTable
{
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores mesh id and particle index pairs, using a spatial hashing function, this is used to
  /// query neighbours in order to speed up collision detection.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::vector<std::pair<GLushort, GLushort>>> m_hashTable;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores a list of particle id's that are within a triangle's bounding box the table is,
  /// indexed using triangle, id plus an offset for the referenced mesh.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::vector<size_t>> m_triangleBoundingBoxHash;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores offsets per mesh, that should be used when indexing into the triangle hash table.
  //-----------------------------------------------------------------------------------------------------
  std::vector<size_t> m_triHashOffset;
};

//-----------------------------------------------------------------------------------------------------
/// @brief Calculates the cell that the given co-ordinate lies within, this is used for spatial hahsing.
/// @param _coord is the 3D co-ordinate who's cell we want to calculate.
/// @return a 3D integer co-ordinate that represents a 3D cell in our simulation space.
//-----------------------------------------------------------------------------------------------------
glm::ivec3 calcCell(const glm::vec3& _coord, const float _cellSize);
//-----------------------------------------------------------------------------------------------------
/// @brief Calculates the hash id for a given 3D cell, used for spatial hahsing.
/// @param _cell is the 3D cell that we want to hash.
/// @return a hash id for the given cell, that can be used to query all particles in this cell from the,
/// hash table.
//-----------------------------------------------------------------------------------------------------
size_t hashCell (const glm::ivec3& _cell, const size_t& _tableSize);
//-----------------------------------------------------------------------------------------------------
/// @brief Wraps the calcCell and hashCell functions, to hash a particle.
/// @param _coord is the 3D co-ordinate of the particle we want to hash.
/// @return a hash id for the given particle, that can be used to query all particles in this cell from,
/// the hash table.
//-----------------------------------------------------------------------------------------------------
size_t hashParticle(const glm::vec3& _coord, const size_t &_tableSize, const float _cellSize);

}

}

#endif // SPATIALHASH_H

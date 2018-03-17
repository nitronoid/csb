#include "gtest/gtest.h"
#include "SpatialHash.h"
#include "UtilMacros.h"

TEST(SpatialHash, calculateCell)
{
  static constexpr auto cellsize = 10.f;

  auto cell = csb::SpatialHash::calcCell(glm::vec3(0.f), cellsize);
  glm::ivec3 expectedCell (0);
  glm::ivec3 notExpectedCell (1);
  EXPECT_VEC3_EQ(expectedCell, cell);
  EXPECT_VEC3_NE(notExpectedCell, cell);

  cell = csb::SpatialHash::calcCell(glm::vec3(100.f, 12.f, 55.f), cellsize);
  expectedCell = glm::ivec3(10, 1, 5);
  notExpectedCell = glm::ivec3(0);
  EXPECT_VEC3_EQ(expectedCell, cell);
  EXPECT_VEC3_NE(notExpectedCell, cell);

  cell = csb::SpatialHash::calcCell(glm::vec3(345.f, -324.f, -68.f), cellsize);
  expectedCell = glm::ivec3(34, -33, -7);
  notExpectedCell = glm::ivec3(0);
  EXPECT_VEC3_EQ(expectedCell, cell);
  EXPECT_VEC3_NE(notExpectedCell, cell);
}

TEST(SpatialHash, hashCell)
{
  static constexpr auto tableSize = 999ul;

  // Expected results calculated using wolfram

  glm::ivec3 cell(0);
  auto hash = csb::SpatialHash::hashCell(cell, tableSize);
  EXPECT_EQ(hash, 0);

  cell = glm::ivec3(2,3,4);
  hash = csb::SpatialHash::hashCell(cell, tableSize);
  EXPECT_EQ(hash, 674);


  cell = glm::ivec3(-100,2,-11);
  hash = csb::SpatialHash::hashCell(cell, tableSize);
  EXPECT_EQ(hash, 236);
}

TEST(SpatialHash, hashParticle)
{
  static constexpr auto cellsize = 10.f;
  static constexpr auto tableSize = 999ul;

  // Expected results calculated using wolfram

  glm::vec3 coord(0.f);
  auto hash = csb::SpatialHash::hashParticle(coord, tableSize, cellsize);
  EXPECT_EQ(hash, 0);

  coord = glm::vec3(100.f, 12.f, 55.f);
  hash = csb::SpatialHash::hashParticle(coord, tableSize, cellsize);
  EXPECT_EQ(hash, 598);


  coord = glm::vec3(345.f, -324.f, -68.f);
  hash = csb::SpatialHash::hashParticle(coord, tableSize, cellsize);
  EXPECT_EQ(hash, 802);


  coord = glm::vec3(-126.f, -89.f, 14.f);
  auto cell = csb::SpatialHash::calcCell(coord, cellsize);
  EXPECT_EQ(csb::SpatialHash::hashCell(cell, tableSize), csb::SpatialHash::hashParticle(coord, tableSize, cellsize));
}

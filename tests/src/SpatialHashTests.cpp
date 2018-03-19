#include "gtest/gtest.h"
#include "SpatialHash.h"
#include "UtilMacros.h"

TEST(SpatialHash, calculateCell)
{
  static constexpr auto cellsize = 10.f;
  static constexpr auto celloffset = 10000.f;

  auto cell = csb::SpatialHash::calcCell(glm::vec3(0.f), cellsize, celloffset);
  glm::ivec3 expectedCell (1000);
  glm::ivec3 notExpectedCell (1);
  EXPECT_VEC3_EQ(expectedCell, cell);
  EXPECT_VEC3_NE(notExpectedCell, cell);

  cell = csb::SpatialHash::calcCell(glm::vec3(100.f, 12.f, 55.f), cellsize, celloffset);
  expectedCell = glm::ivec3(1010, 1001, 1005);
  notExpectedCell = glm::ivec3(100);
  EXPECT_VEC3_EQ(expectedCell, cell);
  EXPECT_VEC3_NE(notExpectedCell, cell);

  cell = csb::SpatialHash::calcCell(glm::vec3(345.f, -324.f, -68.f), cellsize, celloffset);
  expectedCell = glm::ivec3(1034, 967, 993);
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
  static constexpr auto celloffset = 0.f;
  static constexpr auto tableSize = 999ul;

  // Expected results calculated using wolfram

  glm::vec3 coord(0.f);
  auto hash = csb::SpatialHash::hashParticle(coord, tableSize, cellsize, celloffset);
  EXPECT_EQ(hash, 0);

  coord = glm::vec3(100.f, 12.f, 55.f);
  hash = csb::SpatialHash::hashParticle(coord, tableSize, cellsize, celloffset);
  EXPECT_EQ(hash, 598);


  coord = glm::vec3(345.f, -324.f, -68.f);
  hash = csb::SpatialHash::hashParticle(coord, tableSize, cellsize, celloffset);
  EXPECT_EQ(hash, 802);


  coord = glm::vec3(-126.f, -89.f, 14.f);
  auto cell = csb::SpatialHash::calcCell(coord, cellsize, celloffset);
  EXPECT_EQ(csb::SpatialHash::hashCell(cell, tableSize), csb::SpatialHash::hashParticle(coord, tableSize, cellsize, celloffset));
}

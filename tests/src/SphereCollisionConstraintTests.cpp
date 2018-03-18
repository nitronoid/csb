#include "gtest/gtest.h"
#include "SphereCollisionConstraint.h"
#include "UtilMacros.h"


TEST(SphereCollisionConstraint, constructor)
{
  const glm::vec3 centre(0.f, 0.f, 0.f);
  csb::SphereCollisionConstraint s(centre, 1.f);
  EXPECT_VEC3_EQ(centre, s.getCentre());
  EXPECT_EQ(1.f, s.getRadius());


  csb::SphereCollisionConstraint sCopy(s);
  EXPECT_VEC3_EQ(s.getCentre(), sCopy.getCentre());
  EXPECT_EQ(s.getRadius(), sCopy.getRadius());

  csb::SphereCollisionConstraint sMove(std::move(s));
  EXPECT_VEC3_EQ(sCopy.getCentre(), sMove.getCentre());
  EXPECT_EQ(sCopy.getRadius(), sMove.getRadius());
}

TEST(SphereCollisionConstraint, assignment)
{
  const glm::vec3 centre(0.f, 0.f, 0.f);
  csb::SphereCollisionConstraint s(centre, 1.f);
  EXPECT_VEC3_EQ(centre, s.getCentre());
  EXPECT_EQ(1.f, s.getRadius());


  csb::SphereCollisionConstraint sCopy;
  sCopy = s;
  EXPECT_VEC3_EQ(s.getCentre(), sCopy.getCentre());
  EXPECT_EQ(s.getRadius(), sCopy.getRadius());

  csb::SphereCollisionConstraint sMove;
  sMove = std::move(s);
  EXPECT_VEC3_EQ(sCopy.getCentre(), sMove.getCentre());
  EXPECT_EQ(sCopy.getRadius(), sMove.getRadius());
}

TEST(SphereCollisionConstraint, boundingCells)
{
  csb::SphereCollisionConstraint s({0.f, 0.f, 0.f}, 1.f);
  s.setCellSize(1.f);
  s.setHashTableSize(99);
  s.init();

  const auto& lowCells = s.cells();
  // cells from -1 to 1 in x, y and z, hence the result is 3^3 = 27
  EXPECT_EQ(27, lowCells.size());

  s.setCentre({100.f, 5.f, -5.5f});
  s.setRadius(50.5f);
  s.setCellSize(15.f);
  s.setHashTableSize(999);
  s.updateBoundingBoxCells();

  const auto& highCells = s.cells();
  // this has 8x8x8 cells so we expect 512 in total
  EXPECT_EQ(512, highCells.size());
}



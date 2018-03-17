#include "gtest/gtest.h"
#include "BendingConstraint.h"
#include "gtx/fast_square_root.hpp"
#include "UtilMacros.h"

TEST(BendingConstraint, constructor)
{
  std::vector<glm::vec3> testVertices {
    {0.f, 0.f, 0.f},
    {1.f, 1.f, 0.f},
    {0.f, 2.f, 0.f}
  };
  std::vector<csb::Particle> testParticles {
    csb::Particle{testVertices[0], 1.f},
    csb::Particle{testVertices[1], 1.f},
    csb::Particle{testVertices[2], 1.f}
  };

  csb::BendingConstraint b(0, 1, 2, 0.f, 0.f, testParticles);
  EXPECT_EQ(0, b.getParticleIndex(0));
  EXPECT_EQ(1, b.getParticleIndex(1));
  EXPECT_EQ(2, b.getParticleIndex(2));
  EXPECT_EQ(0.25f, b.getParticleWeight(0));
  EXPECT_EQ(0.25f, b.getParticleWeight(1));
  EXPECT_EQ(0.25f, b.getParticleWeight(2));
  EXPECT_EQ(0.f, b.getRest());
  EXPECT_EQ(0.f, b.getStiffness());

  csb::BendingConstraint bCopy(b);
  EXPECT_EQ(0, bCopy.getParticleIndex(0));
  EXPECT_EQ(1, bCopy.getParticleIndex(1));
  EXPECT_EQ(2, bCopy.getParticleIndex(2));
  EXPECT_EQ(0.25f, bCopy.getParticleWeight(0));
  EXPECT_EQ(0.25f, bCopy.getParticleWeight(1));
  EXPECT_EQ(0.25f, bCopy.getParticleWeight(2));
  EXPECT_EQ(0.f, bCopy.getRest());
  EXPECT_EQ(0.f, bCopy.getStiffness());


  csb::BendingConstraint bMove(std::move(b));
  EXPECT_EQ(0, bMove.getParticleIndex(0));
  EXPECT_EQ(1, bMove.getParticleIndex(1));
  EXPECT_EQ(2, bMove.getParticleIndex(2));
  EXPECT_EQ(0.25f, bMove.getParticleWeight(0));
  EXPECT_EQ(0.25f, bMove.getParticleWeight(1));
  EXPECT_EQ(0.25f, bMove.getParticleWeight(2));
  EXPECT_EQ(0.f, bMove.getRest());
  EXPECT_EQ(0.f, bMove.getStiffness());
}

TEST(BendingConstraint, assignment)
{
  std::vector<glm::vec3> testVertices {
    {0.f, 0.f, 0.f},
    {1.f, 1.f, 0.f},
    {0.f, 2.f, 0.f}
  };
  std::vector<csb::Particle> testParticles {
    csb::Particle{testVertices[0], 1.f},
    csb::Particle{testVertices[1], 1.f},
    csb::Particle{testVertices[2], 1.f}
  };

  csb::BendingConstraint b(0, 1, 2, 0.f, 0.f, testParticles);
  EXPECT_EQ(0, b.getParticleIndex(0));
  EXPECT_EQ(1, b.getParticleIndex(1));
  EXPECT_EQ(2, b.getParticleIndex(2));
  EXPECT_EQ(0.25f, b.getParticleWeight(0));
  EXPECT_EQ(0.25f, b.getParticleWeight(1));
  EXPECT_EQ(0.25f, b.getParticleWeight(2));
  EXPECT_EQ(0.f, b.getRest());
  EXPECT_EQ(0.f, b.getStiffness());

  csb::BendingConstraint bCopy;
  bCopy = b;
  EXPECT_EQ(0, bCopy.getParticleIndex(0));
  EXPECT_EQ(1, bCopy.getParticleIndex(1));
  EXPECT_EQ(2, bCopy.getParticleIndex(2));
  EXPECT_EQ(0.25f, bCopy.getParticleWeight(0));
  EXPECT_EQ(0.25f, bCopy.getParticleWeight(1));
  EXPECT_EQ(0.25f, bCopy.getParticleWeight(2));
  EXPECT_EQ(0.f, bCopy.getRest());
  EXPECT_EQ(0.f, bCopy.getStiffness());


  csb::BendingConstraint bMove;
  bMove = std::move(b);
  EXPECT_EQ(0, bMove.getParticleIndex(0));
  EXPECT_EQ(1, bMove.getParticleIndex(1));
  EXPECT_EQ(2, bMove.getParticleIndex(2));
  EXPECT_EQ(0.25f, bMove.getParticleWeight(0));
  EXPECT_EQ(0.25f, bMove.getParticleWeight(1));
  EXPECT_EQ(0.25f, bMove.getParticleWeight(2));
  EXPECT_EQ(0.f, bMove.getRest());
  EXPECT_EQ(0.f, bMove.getStiffness());
}

TEST(BendingConstraint, projection)
{
  std::vector<glm::vec3> testVertices {
    {0.f, 0.f, 0.f},
    {0.f, 2.f, 0.f},
    {3.f, 1.f, 0.f}
  };
  std::vector<csb::Particle> testParticles {
    csb::Particle{testVertices[0], 1.f},
    csb::Particle{testVertices[1], 1.f},
    csb::Particle{testVertices[2], 1.f}
  };


  static constexpr float third = 1.0f / 3.0f;
  auto centre = third * (testVertices[0] + testVertices[1] + testVertices[2]);
  csb::BendingConstraint b(0, 1, 2, glm::fastDistance(testVertices[2], centre), 1.f, testParticles);

  // By doubling the "height" of the triangle, we can observe all the x components,
  // converge together. The end result is that they have shifted towards the new centroid,
  // but their relative differences are unchanged.
  testVertices[2].x = 6.f;
  b.project(testParticles);

  const auto expected1 = glm::vec3(1.f, 0.f, 0.f);
  const auto expected2 = glm::vec3(1.f, 2.f, 0.f);
  const auto expected3 = glm::vec3(4.f, 1.f, 0.f);
  EXPECT_VEC3_EQ(expected1, testVertices[0]);
  EXPECT_VEC3_EQ(expected2, testVertices[1]);
  EXPECT_VEC3_EQ(expected3, testVertices[2]);
}

TEST(BendingConstraint, weightedProjection)
{
  std::vector<glm::vec3> testVertices {
    {0.f, 0.f, 0.f},
    {0.f, 2.f, 0.f},
    {3.f, 1.f, 0.f}
  };
  std::vector<csb::Particle> testParticles {
    csb::Particle{testVertices[0], 1.f},
    csb::Particle{testVertices[1], 1.f},
    csb::Particle{testVertices[2], 0.f}
  };


  static constexpr float third = 1.0f / 3.0f;
  auto centre = third * (testVertices[0] + testVertices[1] + testVertices[2]);
  csb::BendingConstraint b(0, 1, 2, glm::fastDistance(testVertices[2], centre), 1.f, testParticles);

  // When the constraint is weighted towards the tip, we can see that the same correction takes place,
  // however now the entire load has been shifted to the other vertices,
  testVertices[2].x = 6.f;
  b.project(testParticles);

  const auto expected1 = glm::vec3(2.f, 0.f, 0.f);
  const auto expected2 = glm::vec3(2.f, 2.f, 0.f);
  const auto expected3 = glm::vec3(6.f, 1.f, 0.f);
  EXPECT_VEC3_EQ(expected1, testVertices[0]);
  EXPECT_VEC3_EQ(expected2, testVertices[1]);
  EXPECT_VEC3_EQ(expected3, testVertices[2]);
}

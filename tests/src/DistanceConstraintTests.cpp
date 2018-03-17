#include "gtest/gtest.h"
#include "DistanceConstraint.h"
#include "UtilMacros.h"


TEST(DistanceConstraint, constructor)
{
  csb::DistanceConstraint d(0, 1, 0.f);
  EXPECT_EQ(0, d.getParticleIndex1());
  EXPECT_EQ(1, d.getParticleIndex2());
  EXPECT_EQ(0.f, d.getRest());

  csb::DistanceConstraint dCopy(d);
  EXPECT_EQ(0, dCopy.getParticleIndex1());
  EXPECT_EQ(1, dCopy.getParticleIndex2());
  EXPECT_EQ(0.f, dCopy.getRest());

  csb::DistanceConstraint dMove(std::move(d));
  EXPECT_EQ(0, dMove.getParticleIndex1());
  EXPECT_EQ(1, dMove.getParticleIndex2());
  EXPECT_EQ(0.f, dMove.getRest());
}

TEST(DistanceConstraint, assignment)
{
  csb::DistanceConstraint d(0, 1, 0.f);
  EXPECT_EQ(0, d.getParticleIndex1());
  EXPECT_EQ(1, d.getParticleIndex2());
  EXPECT_EQ(0.f, d.getRest());

  csb::DistanceConstraint dCopy;
  dCopy = d;
  EXPECT_EQ(0, dCopy.getParticleIndex1());
  EXPECT_EQ(1, dCopy.getParticleIndex2());
  EXPECT_EQ(0.f, dCopy.getRest());

  csb::DistanceConstraint dMove;
  dMove = std::move(d);
  EXPECT_EQ(0, dMove.getParticleIndex1());;
  EXPECT_EQ(1, dMove.getParticleIndex2());
  EXPECT_EQ(0.f, dMove.getRest());
}

TEST(DistanceConstraint, projection)
{
  std::vector<glm::vec3> testVertices {
    {0.f, 0.f, 0.f},
    {0.f, 2.f, 0.f}
  };
  std::vector<csb::Particle> testParticles {
    csb::Particle{testVertices[0], 1.f},
    csb::Particle{testVertices[1], 1.f}
  };


  // Set the constrained distance to half the current distance
  csb::DistanceConstraint d(0, 1, 1.f);

  d.project(testParticles);

  const auto expected1 = glm::vec3(0.f, 0.5f, 0.f);
  const auto expected2 = glm::vec3(0.f, 1.5f, 0.f);
  EXPECT_VEC3_EQ(expected1, testVertices[0]);
  EXPECT_VEC3_EQ(expected2, testVertices[1]);
}

TEST(DistanceConstraint, weightedProjection)
{
  std::vector<glm::vec3> testVertices {
    {0.f, 0.f, 0.f},
    {0.f, 2.f, 0.f}
  };
  std::vector<csb::Particle> testParticles {
    csb::Particle{testVertices[0], 0.f},
    csb::Particle{testVertices[1], 1.f}
  };


  // Set the constrained distance to half the current distance
  csb::DistanceConstraint d(0, 1, 1.f);

  d.project(testParticles);

  const auto expected1 = glm::vec3(0.f, 0.0f, 0.f);
  const auto expected2 = glm::vec3(0.f, 1.0f, 0.f);
  EXPECT_VEC3_EQ(expected1, testVertices[0]);
  EXPECT_VEC3_EQ(expected2, testVertices[1]);
}

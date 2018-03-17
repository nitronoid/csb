#include "gtest/gtest.h"
#include "PinConstraint.h"
#include "UtilMacros.h"

TEST(PinConstraint, constructor)
{
  const auto pos = glm::vec3(0.f);
  csb::PinConstraint p(0, pos);
  EXPECT_EQ(0, p.getParticleIndex());
  EXPECT_VEC3_EQ(pos, p.getPinPosition());

  csb::PinConstraint pCopy(p);
  EXPECT_EQ(0, p.getParticleIndex());
  EXPECT_VEC3_EQ(pos, p.getPinPosition());

  csb::PinConstraint pMove(std::move(p));
  EXPECT_EQ(0, p.getParticleIndex());
  EXPECT_VEC3_EQ(pos, p.getPinPosition());
}

TEST(PinConstraint, assignment)
{
  const auto pos = glm::vec3(0.f);
  csb::PinConstraint p(0, pos);
  EXPECT_EQ(0, p.getParticleIndex());
  EXPECT_VEC3_EQ(pos, p.getPinPosition());

  csb::PinConstraint pCopy;
  pCopy = p;
  EXPECT_EQ(0, p.getParticleIndex());
  EXPECT_VEC3_EQ(pos, p.getPinPosition());

  csb::PinConstraint pMove;
  pMove = std::move(p);
  EXPECT_EQ(0, p.getParticleIndex());
  EXPECT_VEC3_EQ(pos, p.getPinPosition());
}

TEST(PinConstraint, projection)
{

  auto pos = glm::vec3(0.f);
  const auto expectedPos = pos;
  std::vector<csb::Particle> testParticles {{pos, 1.f}};
  // Set the constrained distance to half the current distance
  csb::PinConstraint p(0, pos);
  pos.x = 10.f;
  pos.y = 12.f;
  pos.z = 15.f;
  p.project(testParticles);

  EXPECT_VEC3_EQ(expectedPos, pos);
}


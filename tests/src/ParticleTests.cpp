#include "gtest/gtest.h"
#include "Particle.h"

TEST(Particle, constructor)
{
  glm::vec3 position(0.f);

  csb::Particle p(position, 0.f);
  EXPECT_EQ(position, *p.m_pos);
  EXPECT_EQ(0.f, p.m_invMass);

  csb::Particle pMove(std::move(p));
  EXPECT_EQ(*p.m_pos, *pMove.m_pos);
  EXPECT_EQ(p.m_invMass, pMove.m_invMass);
}

TEST(Particle, manipulation)
{
  glm::vec3 position(0.f);
  glm::vec3 nextPosition(7.5f, 8.5f, 9.5f);

  csb::Particle p(position, 0.f);

  *p.m_pos = nextPosition;
  p.m_invMass = 1.f;
  EXPECT_EQ(nextPosition, *p.m_pos);
  EXPECT_EQ(position, *p.m_pos);
  EXPECT_EQ(glm::vec3(0.f), p.m_prevPos);
  EXPECT_EQ(1.f, p.m_invMass);
}

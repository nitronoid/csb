#include "gtest/gtest.h"
#include "Edge.h"
#include <unordered_set>

TEST(Edge, constructor)
{
  csb::Edge e(0,1);
  EXPECT_EQ(0, e.p.first);
  EXPECT_EQ(1, e.p.second);

  csb::Edge eCopy(e);
  EXPECT_EQ(e.p.first,  eCopy.p.first);
  EXPECT_EQ(e.p.second, eCopy.p.second);

  csb::Edge eMove(std::move(e));
  EXPECT_EQ(eCopy.p.first,  eMove.p.first);
  EXPECT_EQ(eCopy.p.second, eMove.p.second);
}

TEST(Edge, assignment)
{
  csb::Edge e(0,1);
  EXPECT_EQ(0, e.p.first);
  EXPECT_EQ(1, e.p.second);

  csb::Edge eCopy;
  eCopy = e;
  EXPECT_EQ(e.p.first,  eCopy.p.first);
  EXPECT_EQ(e.p.second, eCopy.p.second);

  csb::Edge eMove;
  eMove = std::move(e);
  EXPECT_EQ(eCopy.p.first,  eMove.p.first);
  EXPECT_EQ(eCopy.p.second, eMove.p.second);
}

TEST(Edge, sorted)
{
  csb::Edge e(10,0);
  EXPECT_TRUE(e.p.first < e.p.second);
  EXPECT_EQ(0, e.p.first);
  EXPECT_EQ(10, e.p.second);
}

TEST(Edge, equality)
{
  csb::Edge a(10, 0);
  csb::Edge b(0, 10);
  csb::Edge c(0, 10);
  csb::Edge d(5, 10);

  EXPECT_TRUE(a == b);
  EXPECT_TRUE(a == c);
  EXPECT_TRUE(b == c);

  EXPECT_FALSE(a == d);
  EXPECT_FALSE(b == d);
  EXPECT_FALSE(c == d);
}

TEST(Edge, hash)
{
  std::unordered_set<csb::Edge> set;
  csb::Edge e(1,2);
  set.insert(e);

  EXPECT_TRUE(*set.begin() == e);
}

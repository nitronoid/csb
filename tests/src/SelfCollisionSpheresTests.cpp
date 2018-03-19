#include "gtest/gtest.h"
#include "SelfCollisionSpheresConstraint.h"
#include "DistanceConstraint.h"
#include "gtx/fast_square_root.hpp"
#include "Solver.h"
#include "UtilMacros.h"


TEST(SelfCollisionSpheresConstraint, constructor)
{
  csb::SelfCollisionSpheresConstraint s(1.f);
  EXPECT_EQ(1.f, s.getSphereDiameter());

  csb::SelfCollisionSpheresConstraint sCopy(s);
  EXPECT_EQ(s.getSphereDiameter(), sCopy.getSphereDiameter());

  csb::SelfCollisionSpheresConstraint sMove(std::move(s));
  EXPECT_EQ(sCopy.getSphereDiameter(), sMove.getSphereDiameter());
}

TEST(SelfCollisionSpheresConstraint, assignment)
{
  csb::SelfCollisionSpheresConstraint s(1.f);
  EXPECT_EQ(1.f, s.getSphereDiameter());

  csb::SelfCollisionSpheresConstraint sCopy;
  sCopy = s;
  EXPECT_EQ(s.getSphereDiameter(), sCopy.getSphereDiameter());

  csb::SelfCollisionSpheresConstraint sMove;
  sMove = std::move(s);
  EXPECT_EQ(sCopy.getSphereDiameter(), sMove.getSphereDiameter());
}

TEST(SelfCollisionSpheresConstraint, resolution)
{

  csb::Solver solver;

  std::vector<std::shared_ptr<csb::SimulatedMesh>> meshes  {std::make_shared<csb::SimulatedMesh>()};
  meshes[0]->load("../demo/models/xyplane.obj");
  meshes[0]->init();
  solver.addSimulatedMesh(meshes[0]);
  const auto sphereRadius = meshes[0]->getShortestEdgeLength() * 1.4f;
  solver.addContinuousCollision(new csb::SelfCollisionSpheresConstraint(sphereRadius));

  // Hash the vertices
  csb::SpatialHash::SpatialHashTable hashTable;
  hashTable.m_hashTable.resize(9);
  const auto& verts = meshes[0]->getVertices();

  const auto oldV1 = verts[1];
  // Make sure we are using diagonal verts
  auto unConstrainedDist = glm::fastDistance(verts[1], verts[3]);
  EXPECT_FLOAT_EQ(1.4142135f, unConstrainedDist);

  // Create a distance constraint that will violate our collision constraint
  meshes[0]->addConstraint(new csb::DistanceConstraint(1, 3, 1.f));
  meshes[0]->projectConstraints();

  // Make sure our distance constraint has violated the collision constraint
  const auto constrainedDist = glm::fastDistance(verts[1], verts[3]);
  EXPECT_FLOAT_EQ(1.f, constrainedDist);

  // get the vel so we can acount for it
  const auto vel = oldV1 - verts[1];

  // This should prevent non-neighbours from being closer than the sphereRadius
  solver.step(0.1f);

  // We need to account for the velocity which was added by the distance constraint
  const auto expectedDist = sphereRadius - glm::fastLength(vel);
  unConstrainedDist = glm::fastDistance(verts[1], verts[3]);
  EXPECT_FLOAT_EQ(expectedDist, unConstrainedDist);

  const auto& adjacency = meshes[0]->getAdjacencyInfo();
  const glm::vec3 expectedPos(0.f, 1.f, 0.f);
  for (size_t i = 0; i < verts.size(); ++i)
  {
    const auto& v1 = verts[i];
    const auto& neighbours = adjacency[i];
    std::unordered_set<size_t> neighbourSet(neighbours.begin(), neighbours.end());
    for (size_t j = 0; j < verts.size(); ++j)
    {
      if (i == j || neighbourSet.count(j)) continue;
      const auto& v2 = verts[j];
      const auto dist = glm::fastDistance(v1, v2);
      EXPECT_GE(dist, expectedDist);
    }
  }
}

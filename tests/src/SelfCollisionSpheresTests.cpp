#include "gtest/gtest.h"
#include "SelfCollisionSpheresConstraint.h"
#include "DistanceConstraint.h"
#include "gtx/fast_square_root.hpp"
#include "Solver.h"
#include "UtilMacros.h"


TEST(SelfCollisionSpheresConstraint, constructor)
{
  csb::SelfCollisionSpheresConstraint s(1.f);
  EXPECT_EQ(1.f, s.getSphereRadius());

  csb::SelfCollisionSpheresConstraint sCopy(s);
  EXPECT_EQ(s.getSphereRadius(), sCopy.getSphereRadius());

  csb::SelfCollisionSpheresConstraint sMove(std::move(s));
  EXPECT_EQ(sCopy.getSphereRadius(), sMove.getSphereRadius());
}

TEST(SelfCollisionSpheresConstraint, assignment)
{
  csb::SelfCollisionSpheresConstraint s(1.f);
  EXPECT_EQ(1.f, s.getSphereRadius());

  csb::SelfCollisionSpheresConstraint sCopy;
  sCopy = s;
  EXPECT_EQ(s.getSphereRadius(), sCopy.getSphereRadius());

  csb::SelfCollisionSpheresConstraint sMove;
  sMove = std::move(s);
  EXPECT_EQ(sCopy.getSphereRadius(), sMove.getSphereRadius());
}

TEST(SelfCollisionSpheresConstraint, resolution)
{

  csb::Solver solver;
  csb::SelfCollisionSpheresConstraint s(1.f);

  std::vector<std::shared_ptr<csb::SimulatedMesh>> meshes  {std::make_shared<csb::SimulatedMesh>()};
  meshes[0]->load("../demo/models/xyplane.obj");
  meshes[0]->init();
  solver.addSimulatedMesh(meshes[0]);
  const auto sphereRadius = meshes[0]->getShortestEdgeLength() * 1.4f;
  solver.addContinuousCollision(new csb::SelfCollisionSpheresConstraint(sphereRadius));

  // Hash the vertices
  csb::SpatialHash::SpatialHashTable hashTable;
  hashTable.m_hashTable.resize(9);
  const auto size = meshes[0]->getNVerts();
  const auto& verts = meshes[0]->getVertices();
  for (GLushort i = 0; i < size; ++i)
  {
    hashTable.m_hashTable[csb::SpatialHash::hashParticle(
          verts[i],
          hashTable.m_hashTable.size(),
          meshes[0]->getTotalEdgeLength() / meshes[0]->getNEdges()
          )].emplace_back(0, i);
  }


  // Make sure we are using diagonal verts
  const auto unConstrainedDist = glm::fastDistance(verts[0], verts[2]);
  EXPECT_FLOAT_EQ(1.4142135f, unConstrainedDist);

  // Create a distance constraint that will violate our collision constraint
  meshes[0]->addConstraint(new csb::DistanceConstraint(0, 2, 1.f));
  meshes[0]->projectConstraints();

  // Make sure our distance constraint has violated the collision constraint
  const auto constrainedDist = glm::fastDistance(verts[0], verts[2]);
  EXPECT_FLOAT_EQ(1.f, constrainedDist);

  // This should prevent non-neighbours from being closer than the sphereRadius
  s.project(0, meshes, hashTable);

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
      EXPECT_GE(dist, sphereRadius);
    }
  }
}

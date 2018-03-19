#include "gtest/gtest.h"
#include "SelfCollisionRaysConstraint.h"
#include "DistanceConstraint.h"
#include "gtx/intersect.hpp"
#include "gtx/fast_square_root.hpp"
#include "Solver.h"
#include "UtilMacros.h"

TEST(SelfCollisionRaysConstraint, constructor)
{
  csb::SelfCollisionRaysConstraint s;

  csb::SelfCollisionRaysConstraint sCopy(s);

  csb::SelfCollisionRaysConstraint sMove(std::move(s));
}

TEST(SelfCollisionRaysConstraint, assignment)
{
  csb::SelfCollisionRaysConstraint s;

  csb::SelfCollisionRaysConstraint sCopy;
  sCopy = s;

  csb::SelfCollisionRaysConstraint sMove;
  sMove = std::move(s);
}


TEST(SelfCollisionRaysConstraint, resolution)
{

  csb::Solver solver;
  solver.setTotalForce({0.f, -0.125f, 0.f});
  solver.setDamping(0.f);


  std::vector<std::shared_ptr<csb::SimulatedMesh>> meshes  {std::make_shared<csb::SimulatedMesh>()};
  meshes[0]->load("../demo/models/rayTest.obj");
  meshes[0]->init();
  meshes[0]->setParticleInverseMass(4, 0.f);
  meshes[0]->setParticleInverseMass(5, 0.f);
  meshes[0]->setParticleInverseMass(6, 0.f);
  solver.addSimulatedMesh(meshes[0]);
  solver.addContinuousCollision(new csb::SelfCollisionRaysConstraint);

  // Hash the vertices
  const auto& verts = meshes[0]->getVertices();
  const auto& v = verts[3];

  const glm::vec3 expectedStartPos(0.f, -0.25f, 0.f);
  EXPECT_VEC3_EQ(v, expectedStartPos);

  solver.step(1.f);

  const glm::vec3 expectedSecondPos(0.f, -0.375f, 0.f);
  EXPECT_VEC3_EQ(v, expectedSecondPos);

  solver.step(1.f);

  const glm::vec3 expectedIntersectPos(0.f, -0.625f, 0.f);
  EXPECT_VEC3_EQ(v, expectedIntersectPos);

  solver.step(1.f);

  // Intersection is detected from previous step, and then corrected in this one
  const glm::vec3 expectedEndPos(0.f, -0.5f, 0.f);
  EXPECT_VEC3_EQ(v, expectedEndPos);

}

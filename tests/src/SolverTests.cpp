#include "gtest/gtest.h"
#include "Solver.h"
#include "UtilMacros.h"


TEST(Solver, constructor)
{
  glm::vec3 position(0.f);

  csb::Solver s;
  const glm::vec3 gravity(0.f, -9.81f, 0.f);
  s.setTotalForce(gravity);
  s.setDamping(0.1f);
  EXPECT_FLOAT_EQ(0.1f, s.getDamping());
  EXPECT_VEC3_EQ(gravity, s.getTotalForce());

  csb::Solver sCopy(s);
  EXPECT_FLOAT_EQ(s.getDamping(), sCopy.getDamping());
  EXPECT_VEC3_EQ(s.getTotalForce(), sCopy.getTotalForce());

  csb::Solver sMove(std::move(s));
  EXPECT_FLOAT_EQ(sCopy.getDamping(), sMove.getDamping());
  EXPECT_VEC3_EQ(sCopy.getTotalForce(), sMove.getTotalForce());
}

TEST(Solver, assignment)
{
  glm::vec3 position(0.f);

  csb::Solver s;
  const glm::vec3 gravity(0.f, -9.81f, 0.f);
  s.setTotalForce(gravity);
  s.setDamping(0.1f);
  EXPECT_FLOAT_EQ(0.1f, s.getDamping());
  EXPECT_VEC3_EQ(gravity, s.getTotalForce());

  csb::Solver sCopy;
  sCopy = s;
  EXPECT_FLOAT_EQ(s.getDamping(), sCopy.getDamping());
  EXPECT_VEC3_EQ(s.getTotalForce(), sCopy.getTotalForce());

  csb::Solver sMove;
  sMove = std::move(s);
  EXPECT_FLOAT_EQ(sCopy.getDamping(), sMove.getDamping());
  EXPECT_VEC3_EQ(sCopy.getTotalForce(), sMove.getTotalForce());
}

TEST(Solver, integration)
{
  std::shared_ptr<csb::SimulatedMesh> mesh = std::make_shared<csb::SimulatedMesh>();
  mesh->load("../demo/models/xyplane.obj");
  mesh->init();
  const auto& vertices = mesh->getVertices();

  csb::Solver s;
  const glm::vec3 gravity(0.f, -10.f, 0.f);
  s.setTotalForce(gravity);
  s.setDamping(0.0f);

  s.addTriangleMesh(mesh);
  s.step(0.1f);

  // reduced to y axis, -10 * 0.1 * 0.1 = -0.1
  for (const auto& v : vertices)
  {
   EXPECT_FLOAT_EQ(v.y, -0.1f);
  }

}

TEST(Solver, integration2)
{
  std::shared_ptr<csb::SimulatedMesh> mesh = std::make_shared<csb::SimulatedMesh>();
  mesh->load("../demo/models/xyplane.obj");
  mesh->init();
  const auto& vertices = mesh->getVertices();

  csb::Solver s;
  const glm::vec3 gravity(0.f, -10.f, 0.f);
  s.setTotalForce(gravity);
  s.setDamping(0.0f);

  s.addTriangleMesh(mesh);
  s.step(0.1f);
  s.step(0.1f);
  s.step(0.1f);

  // reduced to y axis, -10 * 0.1 * 0.1 = -0.1
  for (const auto& v : vertices)
  {
   EXPECT_FLOAT_EQ(v.y, -0.6f);
  }

}

TEST(Solver, damping)
{
  std::shared_ptr<csb::SimulatedMesh> mesh = std::make_shared<csb::SimulatedMesh>();
  mesh->load("../demo/models/xyplane.obj");
  mesh->init();
  const auto& vertices = mesh->getVertices();

  csb::Solver s;
  const glm::vec3 gravity(0.f, -10.f, 0.f);
  s.setTotalForce(gravity);
  s.setDamping(1.f);

  s.addTriangleMesh(mesh);
  s.step(0.1f);
  s.step(0.1f);
  s.step(0.1f);

  // reduced to y axis, -10 * 0.1 * 0.1 = -0.1
  for (const auto& v : vertices)
  {
   EXPECT_FLOAT_EQ(v.y, -0.3f);
  }

}


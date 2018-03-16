#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Particle.h"
#include "TriMesh.h"

TEST(Particle, constructor)
{
  glm::vec3 position(0.f);

  csb::Particle p(position, 0.f);
  EXPECT_EQ(position, p.m_pos);
  EXPECT_EQ(0.f, p.m_invMass);

  csb::Particle pMove(std::move(p));
  EXPECT_EQ(p.m_pos, pMove.m_pos);
  EXPECT_EQ(p.m_invMass, pMove.m_invMass);
}

TEST(Particle, manipulation)
{
  glm::vec3 position(0.f);
  glm::vec3 nextPosition(7.5f, 8.5f, 9.5f);

  csb::Particle p(position, 0.f);

  p.m_pos = nextPosition;
  p.m_invMass = 1.f;
  EXPECT_EQ(nextPosition, p.m_pos);
  EXPECT_EQ(position, p.m_pos);
  EXPECT_EQ(glm::vec3(0.f), p.m_prevPos);
  EXPECT_EQ(1.f, p.m_invMass);
}

TEST(TriMesh, constructor)
{
  csb::TriMesh mesh;

  EXPECT_TRUE(0 == mesh.getNVerts());
  EXPECT_TRUE(0 == mesh.getNVertData() && 0 == mesh.getNAttribData(csb::VERTEX));
  EXPECT_TRUE(0 == mesh.getNUVs());
  EXPECT_TRUE(0 == mesh.getNUVData() && 0 == mesh.getNAttribData(csb::UV));
  EXPECT_TRUE(0 == mesh.getNNorms());
  EXPECT_TRUE(0 == mesh.getNNormData() && 0 == mesh.getNAttribData(csb::NORMAL));
  EXPECT_TRUE(0 == mesh.getNIndices());
  EXPECT_TRUE(0 == mesh.getNIndicesData());
  EXPECT_TRUE(0 == mesh.getNData());
  EXPECT_TRUE(0 == mesh.getNEdges());
}


TEST(TriMesh, loadcube)
{
  csb::TriMesh mesh;
  mesh.load("../demo/models/cube.obj");

  // Data compared to stats reported by Maya.
  EXPECT_EQ(8, mesh.getNVerts());
  EXPECT_EQ(24, mesh.getNVertData());
  EXPECT_EQ(mesh.getNVertData(), mesh.getNAttribData(csb::VERTEX));
  EXPECT_EQ(0, mesh.getNUVs());
  EXPECT_EQ(0, mesh.getNUVData());
  EXPECT_EQ(mesh.getNUVData(), mesh.getNAttribData(csb::UV));
  EXPECT_EQ(0 , mesh.getNNorms());
  EXPECT_EQ(0, mesh.getNNormData());
  EXPECT_EQ(mesh.getNNormData(), mesh.getNAttribData(csb::NORMAL));
  EXPECT_EQ(36, mesh.getNIndices());
  EXPECT_EQ(36, mesh.getNIndicesData());
  EXPECT_EQ(24, mesh.getNData());
  EXPECT_EQ(18, mesh.getNEdges());

  const auto& adjacency = mesh.getAdjacencyInfo();
  EXPECT_EQ(8, adjacency.size());

  // Cube adjacency from Maya
  EXPECT_EQ(5, adjacency[0].size());
  EXPECT_EQ(4, adjacency[1].size());
  EXPECT_EQ(4, adjacency[2].size());
  EXPECT_EQ(5, adjacency[3].size());
  EXPECT_EQ(5, adjacency[4].size());
  EXPECT_EQ(4, adjacency[5].size());
  EXPECT_EQ(4, adjacency[6].size());
  EXPECT_EQ(5, adjacency[7].size());

}

TEST(TriMesh, loadplane)
{
  csb::TriMesh mesh;
  mesh.load("../demo/models/hdPlane.obj");

  // Data compared to stats reported by Maya.
  EXPECT_EQ(961, mesh.getNVerts());
  EXPECT_EQ(2883, mesh.getNVertData());
  EXPECT_EQ(mesh.getNVertData(), mesh.getNAttribData(csb::VERTEX));
  EXPECT_EQ(961, mesh.getNUVs());
  EXPECT_EQ(1922, mesh.getNUVData());
  EXPECT_EQ(mesh.getNUVData(), mesh.getNAttribData(csb::UV));
  EXPECT_EQ(961 , mesh.getNNorms());
  EXPECT_EQ(2883, mesh.getNNormData());
  EXPECT_EQ(mesh.getNNormData(), mesh.getNAttribData(csb::NORMAL));
  EXPECT_EQ(5400, mesh.getNIndices());
  EXPECT_EQ(5400, mesh.getNIndicesData());
  EXPECT_EQ(7688, mesh.getNData());
  EXPECT_EQ(2760, mesh.getNEdges());
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

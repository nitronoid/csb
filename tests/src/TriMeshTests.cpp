#include "gtest/gtest.h"
#include "TriMesh.h"

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

}

TEST(TriMesh, reset)
{
  csb::TriMesh mesh;
  mesh.load("../demo/models/cube.obj");
  mesh.reset();

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

TEST(TriMesh, copyAndMove)
{
  csb::TriMesh mesh;
  mesh.load("../demo/models/cube.obj");

  csb::TriMesh copy = mesh;

  // Data compared to stats reported by Maya.
  EXPECT_EQ(copy.getNVerts(), mesh.getNVerts());
  EXPECT_EQ(copy.getNVertData(), mesh.getNVertData());
  EXPECT_EQ(copy.getNVertData(), mesh.getNVertData());
  EXPECT_EQ(copy.getNUVs(), mesh.getNUVs());
  EXPECT_EQ(copy.getNUVData(), mesh.getNUVData());
  EXPECT_EQ(copy.getNUVData(), mesh.getNUVData());
  EXPECT_EQ(copy.getNNorms(), mesh.getNNorms());
  EXPECT_EQ(copy.getNNormData(), mesh.getNNormData());
  EXPECT_EQ(copy.getNNormData(), mesh.getNNormData());
  EXPECT_EQ(copy.getNIndices(), mesh.getNIndices());
  EXPECT_EQ(copy.getNIndicesData(), mesh.getNIndicesData());
  EXPECT_EQ(copy.getNData(), mesh.getNData());
  EXPECT_EQ(copy.getNEdges(), mesh.getNEdges());

  csb::TriMesh move = std::move(mesh);

  // Data compared to stats reported by Maya.
  EXPECT_EQ(move.getNVerts(), copy.getNVerts());
  EXPECT_EQ(move.getNVertData(), copy.getNVertData());
  EXPECT_EQ(move.getNVertData(), copy.getNVertData());
  EXPECT_EQ(move.getNUVs(), copy.getNUVs());
  EXPECT_EQ(move.getNUVData(), copy.getNUVData());
  EXPECT_EQ(move.getNUVData(), copy.getNUVData());
  EXPECT_EQ(move.getNNorms(), copy.getNNorms());
  EXPECT_EQ(move.getNNormData(), copy.getNNormData());
  EXPECT_EQ(move.getNNormData(), copy.getNNormData());
  EXPECT_EQ(move.getNIndices(), copy.getNIndices());
  EXPECT_EQ(move.getNIndicesData(), copy.getNIndicesData());
  EXPECT_EQ(move.getNData(), copy.getNData());
  EXPECT_EQ(move.getNEdges(), copy.getNEdges());

  copy = move;

  // Data compared to stats reported by Maya.
  EXPECT_EQ(copy.getNVerts(), move.getNVerts());
  EXPECT_EQ(copy.getNVertData(), move.getNVertData());
  EXPECT_EQ(copy.getNVertData(), move.getNVertData());
  EXPECT_EQ(copy.getNUVs(), move.getNUVs());
  EXPECT_EQ(copy.getNUVData(), move.getNUVData());
  EXPECT_EQ(copy.getNUVData(), move.getNUVData());
  EXPECT_EQ(copy.getNNorms(), move.getNNorms());
  EXPECT_EQ(copy.getNNormData(), move.getNNormData());
  EXPECT_EQ(copy.getNNormData(), move.getNNormData());
  EXPECT_EQ(copy.getNIndices(), move.getNIndices());
  EXPECT_EQ(copy.getNIndicesData(), move.getNIndicesData());
  EXPECT_EQ(copy.getNData(), move.getNData());
  EXPECT_EQ(copy.getNEdges(), move.getNEdges());
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

TEST(TriMesh, adjacency)
{
  csb::TriMesh mesh;
  mesh.load("../demo/models/cube.obj");

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

TEST(TriMesh, access)
{
  csb::TriMesh mesh;
  mesh.load("../demo/models/cube.obj");

  EXPECT_EQ(mesh.getVertices().size(), mesh.getNVerts());
  EXPECT_EQ(mesh.getUVs().size(), mesh.getNUVs());
  EXPECT_EQ(mesh.getNormals().size(), mesh.getNNorms());
  EXPECT_EQ(mesh.getIndices().size(), mesh.getNIndices());

  const auto vertData = mesh.getVertexData();
  const auto& verts = mesh.getVertices();
  const auto numVerts = mesh.getNVerts();
  for (size_t i = 0; i < numVerts; ++i)
  {
    const auto index = i * 3;
    EXPECT_EQ(*(vertData + index), verts[i].x);
    EXPECT_EQ(*(vertData + index + 1), verts[i].y);
    EXPECT_EQ(*(vertData + index + 2), verts[i].z);
  }

  const auto uvData = mesh.getUVsData();
  const auto& uvs = mesh.getUVs();
  const auto numUVs = mesh.getNUVs();
  for (size_t i = 0; i < numUVs; ++i)
  {
    const auto index = i * 2;
    EXPECT_EQ(*(uvData + index), uvs[i].x);
    EXPECT_EQ(*(uvData + index + 1), uvs[i].y);
  }

  const auto normalData = mesh.getNormalsData();
  const auto& norms = mesh.getNormals();
  const auto numNorms = mesh.getNNorms();
  for (size_t i = 0; i < numNorms; ++i)
  {
    const auto index = i * 3;
    EXPECT_EQ(*(normalData + index), norms[i].x);
    EXPECT_EQ(*(normalData + index + 1), norms[i].y);
    EXPECT_EQ(*(normalData + index + 2), norms[i].z);
  }
}

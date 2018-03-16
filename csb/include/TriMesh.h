#ifndef TRIMESH_H
#define TRIMESH_H

#include <QOpenGLFunctions>
#include <vector>
#include <string>
#include "vec3.hpp"
#include "vec2.hpp"

//-------------------------------------------------------------------------------------------------------
/// @brief used to refer to a section of buffer data
//-------------------------------------------------------------------------------------------------------
namespace MeshAttributes
{
enum Attribute { VERTEX, UV, NORMAL };
}

class TriMesh
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Virtual default destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~TriMesh() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to load a mesh from a file path.
  /// @param [in] _fname is the path to the mesh file.
  /// @param [in] _meshNum is the index of the mesh in the file's scene.
  //-----------------------------------------------------------------------------------------------------
  void load(const std::string &_fname, const size_t &_meshId = 0);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to reset the mesh arrays.
  //-----------------------------------------------------------------------------------------------------
  void reset();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the indices array for use with openGL buffers.
  /// @return A pointer to the first element in the indices array.
  //-----------------------------------------------------------------------------------------------------
  const GLushort *getIndicesData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the vertex array for use with openGL buffers.
  /// @return A pointer to the first element in the vertex array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getVertexData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the normal array for use with openGL buffers.
  /// @return A pointer to the first element in the normal array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getNormalsData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the UV array for use with openGL buffers.
  /// @return A pointer to the first element in the UV array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getUVsData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the specified attribute array for use with
  /// openGL buffers.
  /// @return A pointer to the first element in an attribute array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getAttribData(const MeshAttributes::Attribute _attrib) const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the number of vertices.
  /// @return The size of our vertex array.
  //-----------------------------------------------------------------------------------------------------
  size_t getNVerts() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the number of indices.
  /// @return The size of our index array.
  //-----------------------------------------------------------------------------------------------------
  size_t getNIndices() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the amount of face indices.
  /// @return The size of our indices array.
  //-----------------------------------------------------------------------------------------------------
  int getNIndicesData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the amount of vertex data elements.
  /// @return The size of our vertex array.
  //-----------------------------------------------------------------------------------------------------
  int getNVertData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the amount of normal data elements.
  /// @return The size of our normal array.
  //-----------------------------------------------------------------------------------------------------
  int getNNormData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the amount of UV data elements.
  /// @return The size of our UV array.
  //-----------------------------------------------------------------------------------------------------
  int getNUVData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the amount of data elements across all arrays.
  /// @return The size of our data arrays combined.
  //-----------------------------------------------------------------------------------------------------
  int getNData() const noexcept;

  std::vector<glm::vec3>& getVertices() noexcept;

  const std::vector<GLushort>& getIndices() const noexcept;

  const std::vector<std::vector<GLushort>>& getAdjacencyInfo() const noexcept;

  size_t getNEdges();

  int getNAttribData(const MeshAttributes::Attribute _attrib) const noexcept;


protected:
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_vertices contains the vertices
  //-----------------------------------------------------------------------------------------------------
  std::vector<glm::vec3> m_vertices;
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_normals contains the normals
  //-----------------------------------------------------------------------------------------------------
  std::vector<glm::vec3> m_normals;
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_uvs contains the UVs
  //-----------------------------------------------------------------------------------------------------
  std::vector<glm::vec2> m_uvs;
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_indices contains the indices
  //-----------------------------------------------------------------------------------------------------
  std::vector<GLushort> m_indices;
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_adjacency stores the adjacent vertex indices for any vertex
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::vector<GLushort>> m_adjacency;
};

#endif // TRIMESH_H
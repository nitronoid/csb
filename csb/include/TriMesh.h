#ifndef TRIMESH_H
#define TRIMESH_H

#include <QOpenGLFunctions>
#include <vector>
#include <unordered_set>
#include <string>
#include "Edge.h"
#include "vec3.hpp"
#include "vec2.hpp"

//-------------------------------------------------------------------------------------------------------
/// @brief used to refer to a section of buffer data
//-------------------------------------------------------------------------------------------------------
namespace csb
{
enum MeshAttribute { VERTEX, UV, NORMAL };
}

namespace csb
{

class TriMesh
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  TriMesh() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  TriMesh(const TriMesh&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  TriMesh& operator=(const TriMesh&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  TriMesh(TriMesh&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  TriMesh& operator=(TriMesh&&) = default;
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
  virtual void reset();
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
  /// @param _attrib is the mesh attribute, who's data will be returned.
  /// @return A pointer to the first element in an attribute array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getAttribData(const csb::MeshAttribute _attrib) const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the number of vertices.
  /// @return The size of our vertex array.
  //-----------------------------------------------------------------------------------------------------
  size_t getNVerts() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the number of UV's.
  /// @return The size of our UV array.
  //-----------------------------------------------------------------------------------------------------
  size_t getNUVs() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the number of normals.
  /// @return The size of our normal array.
  //-----------------------------------------------------------------------------------------------------
  size_t getNNorms() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the number of indices.
  /// @return The size of our index array.
  //-----------------------------------------------------------------------------------------------------
  size_t getNIndices() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the number of edges.
  /// @return The number of vertices + number of faces - 2.
  //-----------------------------------------------------------------------------------------------------
  size_t getNEdges() const noexcept;
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
  /// @brief Used to the amount of data elements for the specified attribute.
  /// @param _attrib is the mesh attribute that we will return the size of.
  /// @return The size of the attribute array for _attrib.
  //-----------------------------------------------------------------------------------------------------
  int getNAttribData(const csb::MeshAttribute _attrib) const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the amount of data elements across all arrays.
  /// @return The size of our data arrays combined.
  //-----------------------------------------------------------------------------------------------------
  int getNData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to get read only access to the vertex array.
  /// @return A const reference to the vertex array, the result should not be used beyond this objects,
  /// lifetime.
  //-----------------------------------------------------------------------------------------------------
  const std::vector<glm::vec3>& getVertices() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to get read only access to the UV array.
  /// @return A const reference to the UV array, the result should not be used beyond this objects,
  /// lifetime.
  //-----------------------------------------------------------------------------------------------------
  const std::vector<glm::vec2>& getUVs() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to get read only access to the Normals array.
  /// @return A const reference to the UV Normals, the result should not be used beyond this objects,
  /// lifetime.
  //-----------------------------------------------------------------------------------------------------
  const std::vector<glm::vec3>& getNormals() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to get read only access to the Index array.
  /// @return A const reference to the Index array, the result should not be used beyond this objects,
  /// lifetime.
  //-----------------------------------------------------------------------------------------------------
  const std::vector<GLushort>& getIndices() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to get read only access to the adjacency information of one vertex.
  /// @return A const reference to a 2D array, containing the neighbour vertices for each vertex.
  //-----------------------------------------------------------------------------------------------------
  const std::vector<std::vector<GLushort>>& getAdjacencyInfo() const noexcept;

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to calculate and store the edges of the mesh, in m_edges.
  //-----------------------------------------------------------------------------------------------------
  void calcEdges();

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
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_adjacency stores the adjacent vertex indices for any vertex
  //-----------------------------------------------------------------------------------------------------
  std::vector<Edge> m_edges;

};
}



#endif // TRIMESH_H

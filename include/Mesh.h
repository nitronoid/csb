#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions>
#include <vector>
#include <string>
#include "MeshVBO.h"
#include "vec3.hpp"
#include "vec2.hpp"

class Mesh
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to load a mesh from a file path.
  /// @param [in] _fname is the path to the mesh file.
  /// @param [in] _meshNum is the index of the mesh in the file's scene.
  //-----------------------------------------------------------------------------------------------------
  void load(const std::string &_fname, const size_t _meshNum = 0);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to reset the mesh arrays.
  //-----------------------------------------------------------------------------------------------------
  void reset();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the vertex array for use with openGL buffers.
  /// @return A pointer to the first element in vertex array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getVertexData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the normal array for use with openGL buffers.
  /// @return A pointer to the first element in normal array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getNormalsData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the UV array for use with openGL buffers.
  /// @return A pointer to the first element in UV array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getUVsData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Gets a pointer to the first data element in the specified attribute array for use with
  /// openGL buffers.
  /// @return A pointer to the first element in an attribute array.
  //-----------------------------------------------------------------------------------------------------
  const GLfloat* getAttribData(const MeshAttributes::Attribute _attrib) const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the the amount of vertex data elements.
  /// @return The size of our vertex array.
  //-----------------------------------------------------------------------------------------------------
  int getNVertData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the the amount of normal data elements.
  /// @return The size of our normal array.
  //-----------------------------------------------------------------------------------------------------
  int getNNormData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the the amount of UV data elements.
  /// @return The size of our UV array.
  //-----------------------------------------------------------------------------------------------------
  int getNUVData() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to the the amount of data elements across all arrays.
  /// @return The size of our data arrays combined.
  //-----------------------------------------------------------------------------------------------------
  int getNData() const noexcept;

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_vertices contains the vertices
  //-----------------------------------------------------------------------------------------------------
  std::vector<glm::vec3> m_vertices;
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_normals contains the normals
  //-----------------------------------------------------------------------------------------------------
  std::vector<glm::vec3> m_normals;
  //-----------------------------------------------------------------------------------------------------
  /// @brief m_uvs contains the UV's
  //-----------------------------------------------------------------------------------------------------
  std::vector<glm::vec2> m_uvs;

};

#endif // MESH_H

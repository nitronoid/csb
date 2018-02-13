#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions>
#include <vector>
#include <string>

class Mesh
{
public:

  void load(const std::string &_fname, const size_t _meshNum = 0);

  void reset();

  /// \brief getVertices returns the reference of the std::vector that contains the vertices
  const GLfloat* getVertexData() const noexcept;

  /// \brief getNormals returns the reference of the std::vector that contains the normals
  const GLfloat* getNormalsData() const noexcept;

  /// \brief getNormals returns the reference of the std::vector that contains the normals
  const GLfloat* getUVsData() const noexcept;

  int getNVertData() const noexcept;

  int getNNormData() const noexcept;

  int getNUVData() const noexcept;

  int getNData() const noexcept;

private:
  /// \brief m_vertices is a std::vector containing the vertices
  std::vector<GLfloat> m_vertices;

  /// \brief m_normals is a std::vector containing the normals
  std::vector<GLfloat> m_normals;

  /// \brief m_normals is a std::vector containing the normals
  std::vector<GLfloat> m_uvs;
};

#endif // MESH_H

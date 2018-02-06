#ifndef AMESH_H
#define AMESH_H

#include <vector>
#include <string>

class AMesh
{
public:

  void load(const std::string &_fname);

  /// \brief getVertices returns the reference of the std::vector that contains the vertices
  const float* getVertexData() const { return &m_vertices[0]; }

  /// \brief getNormals returns the reference of the std::vector that contains the normals
  const float* getNormalsData() const { return &m_normals[0]; }

  /// \brief getNormals returns the reference of the std::vector that contains the normals
  const float* getUVsData() const { return &m_uvs[0]; }

  size_t getNVertData() const { return m_vertices.size(); }

private:
  /// \brief m_vertices is a std::vector containing the vertices
  std::vector<float> m_vertices;

  /// \brief m_normals is a std::vector containing the normals
  std::vector<float> m_normals;

  /// \brief m_normals is a std::vector containing the normals
  std::vector<float> m_uvs;
};

#endif // AMESH_H

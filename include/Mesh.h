#ifndef AMESH_H
#define AMESH_H

#include <vector>
#include <string>

class Mesh
{
public:

  void load(const std::string &_fname, const size_t _meshNum = 0);

  void reset();

  /// \brief getVertices returns the reference of the std::vector that contains the vertices
  const float* getVertexData() const noexcept;

  /// \brief getNormals returns the reference of the std::vector that contains the normals
  const float* getNormalsData() const noexcept;

  /// \brief getNormals returns the reference of the std::vector that contains the normals
  const float* getUVsData() const noexcept;

  size_t getNVertData() const noexcept;

private:
  /// \brief m_vertices is a std::vector containing the vertices
  std::vector<float> m_vertices;

  /// \brief m_normals is a std::vector containing the normals
  std::vector<float> m_normals;

  /// \brief m_normals is a std::vector containing the normals
  std::vector<float> m_uvs;
};

#endif // AMESH_H

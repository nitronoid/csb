#ifndef MESHVBO_H
#define MESHVBO_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <iostream>
#include <vector>
#include <memory>

//-----------------------------------------------------------------------------------------------------
/// @brief used to refer to a section of buffer data
//-----------------------------------------------------------------------------------------------------
namespace MeshAttributes
{
  enum Attribute { VERTEX, NORMAL, UV };
}

class MeshVBO
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief called after construction, used to generate and bind our VBO to store mesh data.
  //-----------------------------------------------------------------------------------------------------
  void init();
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to reset our buffers, removing data from them
  /// @param [in] _size is the size in bytes of the data type we are storing, float would be 4 (probably).
  /// @param [in] _nVert is the amount of elements of _size bytes that we should allocate for the,
  /// vertices in the Vertex Buffer Object.
  /// @param [in] _nNorm is the amount of elements of _size bytes that we should allocate for the,
  /// normals in the Vertex Buffer Object.
  /// @param [in] _nUV is the amount of elements of _size bytes that we should allocate for the,
  /// UV's in the Vertex Buffer Object.
  //-----------------------------------------------------------------------------------------------------
  void reset(const int _size, const int _nVert, const int _nNorm, const int _nUV);
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to add new data into the specified buffer.
  /// @param [in] _address is a pointer to the data we want to store.
  /// @param [in] _section is the section of the buffer we should write our data to.
  //-----------------------------------------------------------------------------------------------------
  void append(const void * _address, const MeshAttributes::Attribute _section);
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to get the size of each data element we are storing.
  /// @return the size of the data elements in our buffer
  //-----------------------------------------------------------------------------------------------------
  int dataSize() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to get the amount of data elements we are storing.
  /// @return the number of data elements in our buffer
  //-----------------------------------------------------------------------------------------------------
  int dataAmount() const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to get the amount of data elements we are storing for a specific section.
  /// @return the number of data elements in the _section of the buffer
  //-----------------------------------------------------------------------------------------------------
  int dataAmount(const MeshAttributes::Attribute _section) const noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to get the offset in bytes of the specified section of data in our buffer.
  /// @return the offset in bytes of _section.
  //-----------------------------------------------------------------------------------------------------
  int offset(const MeshAttributes::Attribute _section) const noexcept;

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief The vertex buffer object that stores our data.
  //-----------------------------------------------------------------------------------------------------
  QOpenGLBuffer m_vbo;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current amount of data elements in each section of m_vbo.
  //-----------------------------------------------------------------------------------------------------
  std::array<int, 3> m_amountOfData = {{0,0,0}};
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current amount of data elements in m_vbo.
  //-----------------------------------------------------------------------------------------------------
  int m_totalAmountOfData = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current size of the stored data members in the buffer.
  //-----------------------------------------------------------------------------------------------------
  int m_size = 0;

};

#endif // MESHVBO_H

#ifndef MESHVBO_H
#define MESHVBO_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <iostream>
#include <vector>
#include <memory>

//-------------------------------------------------------------------------------------------------------
/// @brief used to refer to a section of buffer data
//-------------------------------------------------------------------------------------------------------
namespace MeshAttributes
{
enum Attribute { VERTEX, UV, NORMAL };
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
  /// @param [in] _indicesSize is the size in bytes of the data type used to store indices.
  /// @param [in] _nIndices is the amount of elements of _indicesSize bytes that we should allocate for,
  /// the indices in the Element Buffer Object.
  /// @param [in] _dataSize is the size in bytes of the data type we are storing, float would be 4 (probably).
  /// @param [in] _nVert is the amount of elements of _dataSize bytes that we should allocate for the,
  /// vertices in the Vertex Buffer Object.
  /// @param [in] _nNorm is the amount of elements of _dataSize bytes that we should allocate for the,
  /// normals in the Vertex Buffer Object.
  /// @param [in] _nUV is the amount of elements of _dataSize bytes that we should allocate for the,
  /// UV's in the Vertex Buffer Object.
  //-----------------------------------------------------------------------------------------------------
  void reset(
      const unsigned char _indicesSize,
      const int _nIndices,
      const unsigned char _dataSize,
      const int _nVert,
      const int _nUV,
      const int _nNorm
      );
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to add new data into the specified section of the vertex buffer.
  /// @param [in] _address is a pointer to the data we want to store.
  /// @param [in] _section is the section of the buffer we should write our data to.
  //-----------------------------------------------------------------------------------------------------
  void write(const void * _address, const MeshAttributes::Attribute _section);
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to get the size of each data element we are storing.
  /// @return the size of the data elements in our buffer
  //-----------------------------------------------------------------------------------------------------
  unsigned char dataSize() const noexcept;
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
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to bind our buffers.
  //-----------------------------------------------------------------------------------------------------
  void use();
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to add new index data into the element buffer.
  /// @param [in] io_indices is a pointer to the index data.
  //-----------------------------------------------------------------------------------------------------
  void setIndices(const void *_indices);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief The vertex buffer object that stores our data.
  //-----------------------------------------------------------------------------------------------------
  QOpenGLBuffer m_vbo {QOpenGLBuffer::VertexBuffer};
  //-----------------------------------------------------------------------------------------------------
  /// @brief The element buffer object that stores our face indices.
  //-----------------------------------------------------------------------------------------------------
  QOpenGLBuffer m_ebo {QOpenGLBuffer::IndexBuffer};
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current amount of data elements in each section of m_vbo.
  //-----------------------------------------------------------------------------------------------------
  std::array<int, 3> m_amountOfData = {{0,0,0}};
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current amount of data elements in m_vbo.
  //-----------------------------------------------------------------------------------------------------
  int m_totalAmountOfData = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current number of indices.
  //-----------------------------------------------------------------------------------------------------
  int m_numIndices = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current size of the stored data members in the buffer.
  //-----------------------------------------------------------------------------------------------------
  unsigned char m_dataSize = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current size of the data type used to store indices.
  //-----------------------------------------------------------------------------------------------------
  unsigned char m_indicesSize = 0;


};

#endif // MESHVBO_H

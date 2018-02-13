#ifndef BUFFER_H
#define BUFFER_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <iostream>
#include <vector>
#include <memory>

class Buffer
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief used to specify the type of buffer
  //-----------------------------------------------------------------------------------------------------
  enum BufferType { VERTEX, NORMAL, UV };
  //-----------------------------------------------------------------------------------------------------
  /// @brief called after construction, used to generate and bind our Vertex array pbject, as well as,
  /// generate and bind our Buffer objects, (reset is called for this).
  /// @param [in] _size is the size in bytes of the data type we are storing, float would be 4 (probably).
  /// @param [in] _amountOfData is the number of data elements we will store.
  //-----------------------------------------------------------------------------------------------------
  void init(QObject *parent);
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to reset our buffers, removing data from them
  /// @param [in] _size is the size in bytes of the data type we are storing, float would be 4 (probably).
  /// @param [in] _amountOfData is the number of data elements we will remove.
  //-----------------------------------------------------------------------------------------------------
  void reset(const int _size, const int _nVert, const int _nNorm, const int _nUV);
  //-----------------------------------------------------------------------------------------------------
  /// @brief called to add new data into the specified buffer.
  /// @param [in] _address is a pointer to the data we want to store.
  /// @param [in] _type is the buffer we should append our data to.
  //-----------------------------------------------------------------------------------------------------
  void append(const void * _address, const BufferType _type);
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

  int offset(const BufferType _type) const noexcept;

  void testBindVAO() { m_vao->bind(); }

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Buffer addresses.
  //-----------------------------------------------------------------------------------------------------
  QOpenGLBuffer m_vbo;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Vertex array object.
  //-----------------------------------------------------------------------------------------------------
  std::unique_ptr<QOpenGLVertexArrayObject> m_vao;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current amount of data elements in the buffer.
  //-----------------------------------------------------------------------------------------------------
  int m_amountOfData = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Current size of the stored data members in the buffer.
  //-----------------------------------------------------------------------------------------------------
  int m_size = 0;

  std::array<int, 3> m_amountData = {{0,0,0}};
};

#endif // BUFFER_H

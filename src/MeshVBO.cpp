#include "MeshVBO.h"
#include <numeric>

//-----------------------------------------------------------------------------------------------------
void MeshVBO::init()
{
  // Generate all our required buffers
  m_vbo.create();
  m_vbo.bind();
}
//-----------------------------------------------------------------------------------------------------
void MeshVBO::reset(const int _size, const int _nVert, const int _nNorm, const int _nUV)
{
  // Track the amount of data being stored
  m_amountOfData[VERTEX] = _nVert;
  m_amountOfData[NORMAL] = _nNorm;
  m_amountOfData[UV]     = _nUV;
  m_totalAmountOfData = _nVert + _nNorm + _nUV;
  // Track the size of our stored data
  m_size = _size;
  // For all the buffers, we bind them then clear the data pointer
  m_vbo.bind();
  m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo.allocate(m_size * m_totalAmountOfData);
}
//-----------------------------------------------------------------------------------------------------
void MeshVBO::append(const void *_address, const BufferSection _type)
{
  // Bind the requested buffer, then set it's data pointer
  m_vbo.bind();
  m_vbo.write(offset(_type), _address, m_amountOfData[_type] * m_size);
}
//-----------------------------------------------------------------------------------------------------
int MeshVBO::dataSize() const noexcept
{
  // Returns the size the stored of data elements
  return m_size;
}
//-----------------------------------------------------------------------------------------------------
int MeshVBO::dataAmount() const noexcept
{
  // Returns the amount of data elements
  return m_totalAmountOfData;
}
//-----------------------------------------------------------------------------------------------------
int MeshVBO::dataAmount(const BufferSection _type) const noexcept
{
  // Returns the amount of data elements
  return m_amountOfData[_type];
}
//-----------------------------------------------------------------------------------------------------
int MeshVBO::offset(const BufferSection _type) const noexcept
{
  int offset = 0;
  for (size_t i = 0; i < _type; ++i)
    offset += m_amountOfData[i];
  return offset * m_size;
}


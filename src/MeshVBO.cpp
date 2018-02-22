#include "MeshVBO.h"
#include <numeric>

//-----------------------------------------------------------------------------------------------------
void MeshVBO::init()
{
  // Generate all our required buffers
  m_vbo.create();
  m_vbo.bind();
  m_ebo.create();
  m_ebo.bind();
}
//-----------------------------------------------------------------------------------------------------
void MeshVBO::reset(const unsigned char _indicesSize, const int _nIndices, const unsigned char _dataSize, const int _nVert, const int _nUV, const int _nNorm)
{
  {
    using namespace MeshAttributes;
    // Track the amount of data being stored
    m_amountOfData[VERTEX] = _nVert;
    m_amountOfData[UV]     = _nUV;
    m_amountOfData[NORMAL] = _nNorm;
  }
  m_totalAmountOfData = _nVert + _nNorm + _nUV;
  // Track the size of our stored data
  m_dataSize = _dataSize;
  // For all the buffers, we bind them then clear the data pointer
  m_vbo.bind();
  m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo.allocate(m_dataSize * m_totalAmountOfData);

  m_numIndices = _nIndices;
  m_indicesSize = _indicesSize;
  m_ebo.bind();
  m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_ebo.allocate(_nIndices * m_indicesSize);
}
//-----------------------------------------------------------------------------------------------------
void MeshVBO::write(const void *_address, const MeshAttributes::Attribute _section)
{
  // Bind the requested buffer, then set it's data pointer
  m_vbo.bind();
  m_vbo.write(offset(_section), _address, m_amountOfData[_section] * m_dataSize);
}
//-----------------------------------------------------------------------------------------------------
void MeshVBO::setIndices(const void* _indices)
{
  m_ebo.bind();
  m_ebo.write(0, _indices, m_numIndices * m_indicesSize);
}
//-----------------------------------------------------------------------------------------------------
unsigned char MeshVBO::dataSize() const noexcept
{
  // Returns the size the stored of data elements
  return m_dataSize;
}
//-----------------------------------------------------------------------------------------------------
int MeshVBO::dataAmount() const noexcept
{
  // Returns the amount of data elements
  return m_totalAmountOfData;
}
//-----------------------------------------------------------------------------------------------------
int MeshVBO::dataAmount(const MeshAttributes::Attribute _section) const noexcept
{
  // Returns the amount of data elements
  return m_amountOfData[_section];
}
//-----------------------------------------------------------------------------------------------------
int MeshVBO::offset(const MeshAttributes::Attribute _section) const noexcept
{
  int offset = 0;
  for (size_t i = 0; i < _section; ++i)
    offset += m_amountOfData[i];
  return offset * m_dataSize;
}
//-----------------------------------------------------------------------------------------------------
void MeshVBO::use()
{
  m_vbo.bind();
  m_ebo.bind();
}

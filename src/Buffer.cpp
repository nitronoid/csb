#include "Buffer.h"
#include <numeric>

//-----------------------------------------------------------------------------------------------------
void Buffer::init(QObject* parent)
{
  // Generate and bind a Vertex array object
  m_vao.reset(new QOpenGLVertexArrayObject(parent));
  m_vao->create();
  m_vao->bind();
  // Generate all our required buffers
  m_vbo.create();
  m_vbo.bind();
}
//-----------------------------------------------------------------------------------------------------
void Buffer::reset(const int _size, const int _nVert, const int _nNorm, const int _nUV)
{
  // Track the amount of data being stored
  m_amountData[VERTEX] = _nVert;
  m_amountData[NORMAL] = _nNorm;
  m_amountData[UV]     = _nUV;
  m_amountOfData = _nVert + _nNorm + _nUV;
  // Track the size of our stored data
  m_size = _size;
  // For all the buffers, we bind them then clear the data pointer
  m_vbo.bind();
  m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo.allocate(m_size * m_amountOfData);
  std::cout<<m_vbo.size()<<'\n';
}
//-----------------------------------------------------------------------------------------------------
void Buffer::append(const void *_address, const BufferType _type)
{
  // Bind the requested buffer, then set it's data pointer
  m_vbo.bind();
  m_vbo.write(offset(_type), _address, m_amountData[_type] * m_size);
}
//-----------------------------------------------------------------------------------------------------
int Buffer::dataSize() const noexcept
{
  // Returns the size the stored of data elements
  return m_size;
}
//-----------------------------------------------------------------------------------------------------
int Buffer::dataAmount() const noexcept
{
  // Returns the amount of data elements
  return m_amountOfData;
}
//-----------------------------------------------------------------------------------------------------
int Buffer::offset(const BufferType _type) const noexcept
{
  int offset = 0;
  for (size_t i = 0; i < _type; ++i)
    offset += m_amountData[i];
  return offset * m_size;
}


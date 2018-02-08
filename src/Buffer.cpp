#include "Buffer.h"

//-----------------------------------------------------------------------------------------------------
void Buffer::init(const GLuint _size, const GLuint _amountOfData)
{
  // Generate and bind a Vertex array object
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);
  // Generate all our required buffers
  for (auto& buffer : m_buffers)
    glGenBuffers(1, &buffer);
  // Reset the buffers
  reset(_size, _amountOfData);
}
//-----------------------------------------------------------------------------------------------------
void Buffer::reset(const GLuint _size, const GLuint _amountOfData)
{
  // Track the amount of data being stored
  m_amountOfData = _amountOfData;
  // Track the size of our stored data
  m_size = _size;
  // For all the buffers, we bind them then clear the data pointer
  for (const auto& buffer : m_buffers)
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, m_size * m_amountOfData, nullptr, GL_STATIC_DRAW);
  }
}
//-----------------------------------------------------------------------------------------------------
void Buffer::append(const void *_address, const BufferType _type)
{
  // Bind the requested buffer, then set it's data pointer
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[_type]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, m_size * m_amountOfData, _address);
}
//-----------------------------------------------------------------------------------------------------
GLuint Buffer::dataSize() const noexcept
{
  // Returns the size the stored of data elements
  return m_size;
}
//-----------------------------------------------------------------------------------------------------
GLuint Buffer::dataAmount() const noexcept
{
  // Returns the amount of data elements
  return m_amountOfData;
}
//-----------------------------------------------------------------------------------------------------

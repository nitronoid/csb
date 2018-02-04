#include "Buffer.h"

void Buffer::init(const GLuint _size, GLuint _sizeOfData)
{
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);
  for (auto& buffer : m_buffers)
    glGenBuffers(1, &buffer);
  reset(_size, _sizeOfData);
}


GLuint Buffer::append(const void *_address, GLuint size, BufferType _type)
{
  // keeps track of the index of the elements, it will point to a specific index depending on the case
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[_type]);
  auto currentIndex = m_bufferIds[_type];
  m_elements.push_back(currentIndex);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size * m_sizeOfData, _address);
  return currentIndex; // return the index of the current element
}

void Buffer::reset(const GLuint _size, GLuint _sizeOfData)
{
  m_sizeOfData = _sizeOfData;
  // for all the buffers
  for (const auto& buffer : m_buffers)
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, nullptr, GL_STATIC_DRAW);
  }
  m_elements.clear();
  std::fill(m_bufferIds.begin(), m_bufferIds.end(), 0);
}

GLuint Buffer::dataSize() const noexcept
{
  return m_sizeOfData;
}

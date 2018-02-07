#ifndef BUFFER_H
#define BUFFER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#define LINUX
#endif
#include <iostream>
#include <vector>

class Buffer
{
public:
  enum BufferType { VERTEX, NORMAL, UV };
  void init(const GLuint _size, GLuint _sizeOfData);
  GLuint append(const void * _address, GLuint size, BufferType _type);
	void clear(const int _size, int _sizeOfData);
  void reset(const GLuint _size, GLuint _sizeOfData);
  GLuint dataSize() const noexcept;
private:
  std::vector<GLuint> m_buffers   = {0,0,0};
  std::vector<GLuint> m_bufferIds = {0,0,0};
  GLuint m_vao;
  GLuint m_sizeOfData;
  std::vector<GLuint> m_elements;
};

#endif // BUFFER_H

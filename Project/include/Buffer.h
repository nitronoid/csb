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
	enum class operation {r = 'r', u = 'u', l = 'l', d = 'd'};
	enum BufferType { VERTEX, NORMAL, UV };
  Buffer(); // makes sure that the compiler doesn't generate a default constructor
	Buffer(const int _size, int _sizeOfData);
  int append(const void * _address, int size, BufferType _type);
	void clear(const int _size, int _sizeOfData);
  void reset(const int _size);
private:
	GLuint m_vao;
	GLuint m_vertexBuffer;
	GLuint m_normalBuffer;
	GLuint m_uvBuffer;
	int m_vertex_index;
	int m_normal_index;
	int m_uv_index;
	int m_sizeOfData;
	std::vector<int> m_elements;
};

#endif // BUFFER_H

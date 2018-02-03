#include "Buffer.h"

Buffer::Buffer()
{

}

Buffer::Buffer( const int _size, int _sizeOfData )
{
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_normalBuffer);
	glGenBuffers(1, &m_uvBuffer);
	m_sizeOfData = _sizeOfData;

	// allocate the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,	m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,	m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
	m_vertex_index = 0;
	m_normal_index = 0;
	m_uv_index = 0;

}

int Buffer::append(const void *_address, int size, BufferType _type)
{
	// keeps track of the index of the elements, it will point to a specific index depending on the case
	int * currentIndex;
	int tmp_index = 0;
	switch(_type)
	{
		case(VERTEX):
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
			m_elements.push_back(m_vertex_index);
			currentIndex = &m_vertex_index;
			//std::cout <<"vertex " << *currentIndex <<" "<< size << '\n';
			break;
		case(NORMAL):
			glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
			currentIndex = &m_normal_index;
			//std::cout <<"normals " << *currentIndex <<" "<< size << '\n';
			break;
		case(UV):
			glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
			currentIndex = &m_uv_index;
			break;
	}
	tmp_index = *currentIndex;
	*currentIndex += size;
	glBufferSubData(GL_ARRAY_BUFFER, tmp_index * m_sizeOfData, size * m_sizeOfData, _address);
	return tmp_index; // return the index of the current element
}

void Buffer::reset( const int _size )
{
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,	m_normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,	m_uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
  m_elements.clear();
  m_vertex_index = 0;
  m_normal_index = 0;
}

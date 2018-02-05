#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "Shader.h"

void Shader::init(const std::string &_name, const std::string &_vertex, const std::string &_fragment)
{
  m_shaderProgram = glCreateProgram();
  m_name = _name;

  loadShader(_vertex, GL_VERTEX_SHADER);
  loadShader(_fragment, GL_FRAGMENT_SHADER);

  glLinkProgram(m_shaderProgram);
}

void Shader::loadShader(const std::string &_filename, const GLenum _shaderType)
{
  // creation of the vertex shader
  GLuint newShader = glCreateShader(_shaderType);
  std::string shaderFile = loadShaderFile(_filename);
  const GLchar* shaderFilePtr = static_cast<const GLchar*>(shaderFile.c_str());
  glShaderSource(newShader, 1, &shaderFilePtr, nullptr);
  glCompileShader(newShader);
  glAttachShader(m_shaderProgram, newShader);

  GLint status;
  glGetShaderiv(newShader, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    GLchar buffer[500];
    glGetShaderInfoLog(newShader,
                       500,
                       nullptr,
                       buffer);
    std::string log = "shader did not compile\n";
    log += buffer;
    std::cout<<log<<'\n';
    assert(status);
  }
  glDeleteShader(newShader);
}

std::string Shader::loadShaderFile(std::string _filename)
{
  std::ifstream shaderFile(_filename);
  std::string source ((std::istreambuf_iterator<char>(shaderFile)),
                   std::istreambuf_iterator<char>());
  shaderFile.close();
  source += '\0';
  return source;
}


void Shader::use()
{
  glUseProgram(m_shaderProgram);
}

std::string Shader::getName()
{
  return m_name;
}

GLuint Shader::getShaderProgram()
{
  return m_shaderProgram;
}


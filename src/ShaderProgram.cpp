#include <stdio.h>
#include <iostream>
#include <gtc/type_ptr.hpp>
#include <assert.h>
#include "ShaderProgram.h"

void ShaderProgram::init(const std::string &_vertex, const std::string &_fragment)
{
  m_shaderProgram = glCreateProgram();

  loadShader(_vertex, GL_VERTEX_SHADER);
  loadShader(_fragment, GL_FRAGMENT_SHADER);

  glLinkProgram(m_shaderProgram);
}

ShaderProgram::~ShaderProgram()
{
  clearAllShaders();
  glDeleteProgram(m_shaderProgram);
}

void ShaderProgram::loadShader(const std::string &_filename, const GLenum _shaderType)
{
  // creation of the vertex shader
  GLuint newShader = glCreateShader(_shaderType);
  std::string shaderFile = loadShaderFile(_filename);
  const GLchar* shaderFilePtr = static_cast<const GLchar*>(shaderFile.c_str());
  glShaderSource(newShader, 1, &shaderFilePtr, nullptr);
  glCompileShader(newShader);
  glAttachShader(m_shaderProgram, newShader);
  m_attachedShaders[_shaderType] = newShader;
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
  //glDeleteShader(newShader);
}

void ShaderProgram::clearShader(const GLenum _shaderType)
{
  glDetachShader(m_shaderProgram, m_attachedShaders[_shaderType]);
  m_attachedShaders.erase(_shaderType);
}

void ShaderProgram::clearAllShaders()
{
  for (const auto& attached : m_attachedShaders)
  {
    glDetachShader(m_shaderProgram, attached.second);
  }
  m_attachedShaders.clear();
}

std::string ShaderProgram::loadShaderFile(std::string _filename)
{
  std::ifstream shaderFile(_filename);
  std::string source ((std::istreambuf_iterator<char>(shaderFile)),
                   std::istreambuf_iterator<char>());
  shaderFile.close();
  source += '\0';
  return source;
}

void ShaderProgram::setUniform(const char*_name, const bool _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform1i(location, _v);
}

void ShaderProgram::setUniform(const char*_name, const float _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform1f(location, _v);
}

void ShaderProgram::setUniform(const char*_name, const float _v0, const float _v1)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform2f(location, _v0, _v1);
}

void ShaderProgram::setUniform(const char*_name, const float _v0, const float _v1, const float _v2)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform3f(location, _v0, _v1, _v2);
}

void ShaderProgram::setUniform(const char*_name, const float _v0, const float _v1, const float _v2, const float _v3)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform4f(location, _v0, _v1, _v2, _v3);
}

void ShaderProgram::setUniform(const char*_name, const glm::vec2 _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform2fv(location, 1, glm::value_ptr(_v));
}

void ShaderProgram::setUniform(const char*_name, const glm::vec3 _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform3fv(location, 1, glm::value_ptr(_v));
}

void ShaderProgram::setUniform(const char*_name, const glm::vec4 _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniform4fv(location, 1, glm::value_ptr(_v));
}

void ShaderProgram::setUniform(const char*_name, const glm::mat2 _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(_v));
}

void ShaderProgram::setUniform(const char*_name, const glm::mat3 _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(_v));
}

void ShaderProgram::setUniform(const char*_name, const glm::mat4 _v)
{
  auto location = glGetUniformLocation(m_shaderProgram, _name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(_v));
}


void ShaderProgram::use()
{
  glUseProgram(m_shaderProgram);
}


GLuint ShaderProgram::getShaderProgram()
{
  return m_shaderProgram;
}


#include "ShaderLib.h"

void ShaderLib::createShader(const std::string &_vertexName, const std::string &_fragName)
{
  m_shaders.emplace_back();
}

void ShaderLib::useShader(const size_t _index)
{
  m_currentShader = _index;
  m_shaders[_index].use();
}

ShaderProgram* ShaderLib::getShader(const size_t _index)
{
  return &m_shaders[_index];
}

ShaderProgram* ShaderLib::getCurrentShader()
{
  return &m_shaders[m_currentShader];
}

size_t ShaderLib::getCurrentShaderIndex() const noexcept
{
  return m_currentShader;
}

#ifndef SHADERLIB_H
#define SHADERLIB_H

#include <vector>
#include "ShaderProgram.h"

class ShaderLib
{
public:
  void createShader(const std::string &_vertexName, const std::string &_fragName);
  void useShader(const size_t _index);
  ShaderProgram* getShader(const size_t _index);
  ShaderProgram* getCurrentShader();
  size_t getCurrentShaderIndex() const noexcept;

private:
  std::vector<ShaderProgram> m_shaders;
  size_t m_currentShader = 0;
};

#endif // SHADERLIB_H

#ifndef SHADERLIB_H
#define SHADERLIB_H

#include <vector>
#include <QOpenGLShaderProgram>
#include <memory>

class ShaderLib
{
public:
  ShaderLib() /*: m_shaders(2)*/ {}
  void createShader(const QString &_vertexName, const QString &_fragName);
  void useShader(const size_t _index);
  QOpenGLShaderProgram* getShader(const size_t _index);
  QOpenGLShaderProgram* getCurrentShader();
  size_t getCurrentShaderIndex() const noexcept;

//private:
  std::vector<std::unique_ptr<QOpenGLShaderProgram>> m_shaders;
  size_t m_currentShader = 0;
};

#endif // SHADERLIB_H

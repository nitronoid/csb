#include "ShaderLib.h"

void ShaderLib::createShader(const QString &_vertexName, const QString &_fragName)
{
  QOpenGLShaderProgram *program = new QOpenGLShaderProgram();

  program->addShaderFromSourceFile(QOpenGLShader::Vertex, _vertexName);
  program->addShaderFromSourceFile(QOpenGLShader::Fragment, _fragName);

  program->link();
  m_shaders.emplace_back(program);
}

void ShaderLib::useShader(const size_t _index)
{
  m_currentShader = _index;
  m_shaders[_index]->bind();
}

QOpenGLShaderProgram *ShaderLib::getShader(const size_t _index)
{
  return m_shaders[_index].get();
}

QOpenGLShaderProgram* ShaderLib::getCurrentShader()
{
  return m_shaders[m_currentShader].get();
}

size_t ShaderLib::getCurrentShaderIndex() const noexcept
{
  return m_currentShader;
}

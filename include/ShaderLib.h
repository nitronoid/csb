#ifndef SHADERLIB_H
#define SHADERLIB_H

#include <vector>
#include <unordered_map>
#include <QOpenGLShaderProgram>
#include <memory>

class ShaderLib
{
public:
  std::string loadShaderProg(const QString &_jsonFileName);
  void createShader(const std::string& _name, const QString &_vertexName, const QString &_fragName);
  void useShader(const std::string& _name);
  QOpenGLShaderProgram* getShader(const std::string& _name);
  QOpenGLShaderProgram* getCurrentShader();

private:
  std::unordered_map<std::string, std::unique_ptr<QOpenGLShaderProgram>> m_shaderPrograms;
  QOpenGLShaderProgram* m_currentShader;
};

#endif // SHADERLIB_H

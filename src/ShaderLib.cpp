#include "ShaderLib.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

std::string ShaderLib::loadShaderProg(const QString &_jsonFileName)
{
  // Read in raw file
  QFile file(_jsonFileName);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QByteArray rawData = file.readAll();
  // Parse document
  QJsonDocument doc(QJsonDocument::fromJson(rawData));
  // Get the json object to view
  QJsonObject shaderParts = doc.object();

  // Get a string out from the json
  std::string shaderName = shaderParts["Name"].toString().toStdString();

  // Load the shader if we haven't already
  if (!m_shaderPrograms.count(shaderName))
    createShader(shaderName, shaderParts["Vertex"].toString(), shaderParts["Fragment"].toString());

  return shaderName;
}

void ShaderLib::createShader(const std::string &_name, const QString &_vertexName, const QString &_fragName)
{
  QOpenGLShaderProgram *program = new QOpenGLShaderProgram();

  program->addShaderFromSourceFile(QOpenGLShader::Vertex, _vertexName);
  program->addShaderFromSourceFile(QOpenGLShader::Fragment, _fragName);

  program->link();
  m_shaderPrograms[_name].reset(program);
}

void ShaderLib::useShader(const std::string& _name)
{
  m_currentShader = m_shaderPrograms[_name].get();
  m_currentShader->bind();
}

QOpenGLShaderProgram *ShaderLib::getShader(const std::string& _name)
{
  return m_shaderPrograms[_name].get();
}

QOpenGLShaderProgram* ShaderLib::getCurrentShader()
{
  return m_currentShader;
}


#ifndef Shader_h
#define Shader_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <string>
#include <fstream>

class Shader
{
public:
  Shader() = default;
  Shader(const Shader&) = default;
  Shader& operator=(const Shader&) = default;
  Shader(Shader&&) = default;
  Shader& operator=(Shader&&) = default;
  ~Shader() = default;

  void init(const std::string &_name, const std::string &_vertex, const std::string &_fragment);
  std::string getName();
  GLuint getShaderProgram();

  void use();

private:
	std::string m_name;
  GLuint m_shaderProgram;
  void loadShader(const std::string &_filename, const GLenum _shaderType);
  std::string loadShaderFile(std::string _filename);

};

#endif /* Shader_h */

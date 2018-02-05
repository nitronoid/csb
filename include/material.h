#ifndef MATERIAL_H
#define MATERIAL_H

#include <array>
#include <mat4x4.hpp>

class ShaderProgram;

class Material
{
public:
  Material() = default;
  Material(const Material&) = default;
  Material& operator=(const Material&) = default;
  Material(Material&&) = default;
  Material& operator=(Material&&) = default;
  virtual ~Material() = default;

  virtual void setup(ShaderProgram* _shader) = 0;

  virtual void update(ShaderProgram* _shader) = 0;

  glm::mat4* modelViewMatrix();

protected:
  enum MODEL_MATRIX { MODEL_VIEW, PROJECTION, NORMAL };
  std::array<glm::mat4, 3> m_matrix;
};

#endif // MATERIAL_H

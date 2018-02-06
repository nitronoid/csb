#include "Material.h"


void Material::init(ShaderProgram* _shader)
{
  m_shader = _shader;
}

glm::mat4* Material::modelViewMatrix()
{
  return &m_matrix[MODEL_VIEW];
}

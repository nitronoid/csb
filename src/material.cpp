#include "material.h"

glm::mat4* Material::modelViewMatrix()
{
  return &m_matrix[MODEL_VIEW];
}

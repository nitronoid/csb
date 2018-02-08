#include "Material.h"

//-----------------------------------------------------------------------------------------------------
void Material::init(ShaderProgram* io_shader, std::array<glm::mat4, 3>* io_matrices)
{
  // Setup our pointers
  m_shader = io_shader;
  m_matrices = io_matrices;
}
//-----------------------------------------------------------------------------------------------------

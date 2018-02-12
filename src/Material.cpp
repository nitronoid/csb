#include "Material.h"
#include "ShaderLib.h"

//-----------------------------------------------------------------------------------------------------
void Material::init(ShaderLib *io_shaderLib, const size_t _index, std::array<glm::mat4, 3>* io_matrices)
{
  // Setup our pointers
  m_shaderLib = io_shaderLib;
  m_shaderIndex = _index;
  m_matrices = io_matrices;
}
//-----------------------------------------------------------------------------------------------------

void Material::apply()
{
  m_shaderLib->useShader(m_shaderIndex);
}

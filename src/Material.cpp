#include "Material.h"
#include "ShaderLib.h"

//-----------------------------------------------------------------------------------------------------
void Material::apply()
{
  m_shaderLib->useShader(m_shaderName);
  init();
}
//-----------------------------------------------------------------------------------------------------
void Material::setShaderName(const std::string &_name)
{
  m_shaderName = _name;
}

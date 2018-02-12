#include "MaterialPBR.h"
#include "ShaderProgram.h"
#include "Scene.h"
#include "ShaderLib.h"

void MaterialPBR::init(ShaderLib *io_shaderLib, const size_t _index, std::array<glm::mat4, 3>* io_matrices)
{
  Material::init(io_shaderLib, _index, io_matrices);
  auto shaderPtr = m_shaderLib->getShader(m_shaderIndex);
  shaderPtr->setUniform("albedo", 0.5f, 0.0f, 0.0f);
  shaderPtr->setUniform("ao", 1.0f);
  shaderPtr->setUniform("exposure", 1.0f);
  shaderPtr->setUniform("roughness", 0.5f);
  shaderPtr->setUniform("metallic", 1.0f);

  // Update our matrices
  update();
}

void MaterialPBR::update()
{
  auto shaderPtr = m_shaderLib->getShader(m_shaderIndex);
  shaderPtr->setUniform("camPos", m_cam->getCameraEye());

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    static constexpr std::array<const char*, 3> shaderUniforms = {{"M", "MVP", "N"}};
    // Send all our matrices to the GPU
    for (const auto matrixId : {MODEL_VIEW, PROJECTION, NORMAL})
    {
      shaderPtr->setUniform(shaderUniforms[matrixId], (*m_matrices)[matrixId]);
    }
  }
}

const char* MaterialPBR::vertexName() const
{
  return "shaders/PBRVertex.glsl";
}

const char* MaterialPBR::fragName() const
{
  return "shaders/PBRFragment.glsl";
}


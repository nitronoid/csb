#include "MaterialPBR.h"
#include "ShaderProgram.h"
#include "Scene.h"

void MaterialPBR::init(ShaderProgram* io_shader, std::array<glm::mat4, 3>* io_matrices)
{
  Material::init(io_shader, io_matrices);

  m_shader->setUniform("albedo", 0.5f, 0.0f, 0.0f);
  m_shader->setUniform("ao", 1.0f);
  m_shader->setUniform("exposure", 1.0f);
  m_shader->setUniform("roughness", 0.5f);
  m_shader->setUniform("metallic", 1.0f);

  // Update our matrices
  update();
}

void MaterialPBR::update()
{
  m_shader->setUniform("camPos", m_cam->getCameraEye());

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    static constexpr std::array<const char*, 3> shaderUniforms = {{"M", "MVP", "N"}};
    // Send all our matrices to the GPU
    for (const auto matrixId : {MODEL_VIEW, PROJECTION, NORMAL})
    {
      m_shader->setUniform(shaderUniforms[matrixId], (*m_matrices)[matrixId]);
    }
  }
}

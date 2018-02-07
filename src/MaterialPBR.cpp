#include "MaterialPBR.h"
#include "ShaderProgram.h"
#include "Camera.h"

void MaterialPBR::init(ShaderProgram* _shader)
{
  Material::init(_shader);
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
  m_matrix[PROJECTION] = m_cam->projMatrix() * m_cam->viewMatrix() * m_matrix[MODEL_VIEW];
  m_matrix[NORMAL] = glm::inverse(glm::transpose(m_matrix[MODEL_VIEW]));

  // Send all our matrices to the GPU
  static constexpr std::array<const char*, 3> shaderUniforms = {{"M", "MVP", "N"}};
  for (const auto matrixId : {MODEL_VIEW, PROJECTION, NORMAL})
  {
    m_shader->setUniform(shaderUniforms[matrixId], m_matrix[matrixId]);
  }
}

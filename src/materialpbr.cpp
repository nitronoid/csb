#include "materialpbr.h"
#include "ShaderProgram.h"
#include "Camera.h"

void MaterialPBR::setup(ShaderProgram* _shader)
{
  _shader->setUniform("albedo", 0.5f, 0.0f, 0.0f);
  _shader->setUniform("ao", 1.0f);
  _shader->setUniform("exposure", 1.0f);
  _shader->setUniform("roughness", 0.5f);
  _shader->setUniform("metallic", 1.0f);

  // Update our matrices
  update(_shader);
}

void MaterialPBR::update(ShaderProgram* _shader)
{
  _shader->setUniform("camPos", m_cam->getPosition());
  m_matrix[PROJECTION] = m_cam->projMatrix() * m_cam->viewMatrix() * m_matrix[MODEL_VIEW];
  m_matrix[NORMAL] = glm::inverse(glm::transpose(m_matrix[MODEL_VIEW]));

  // Send all our matrices to the GPU
  static constexpr std::array<const char*, 3> shaderUniforms = {{"M", "MVP", "N"}};
  for (const auto matrixId : {MODEL_VIEW, PROJECTION, NORMAL})
  {
    _shader->setUniform(shaderUniforms[matrixId], m_matrix[matrixId]);
  }
}

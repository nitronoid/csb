#include "MaterialPBR.h"
#include "Scene.h"
#include "ShaderLib.h"

void MaterialPBR::init()
{
  auto shaderPtr = m_shaderLib->getCurrentShader();

  shaderPtr->setUniformValue("albedo", QVector3D{m_albedo.x, m_albedo.y, m_albedo.z});
  shaderPtr->setUniformValue("ao", m_ao);
  shaderPtr->setUniformValue("exposure", m_exposure);
  shaderPtr->setUniformValue("roughness", m_roughness);
  shaderPtr->setUniformValue("metallic", m_metallic);

  // Update our matrices
  update();
}

void MaterialPBR::update()
{
  auto shaderPtr = m_shaderLib->getShader(m_shaderName);
  auto eye = m_cam->getCameraEye();
  shaderPtr->setUniformValue("camPos", QVector3D{eye.x, eye.y, eye.z});

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    static constexpr std::array<const char*, 3> shaderUniforms = {{"M", "MVP", "N"}};
    // Send all our matrices to the GPU
    for (const auto matrixId : {MODEL_VIEW, PROJECTION, NORMAL})
    {
      // Convert from glm to Qt
      QMatrix4x4 qmat(glm::value_ptr((*m_matrices)[matrixId]));
      // Need to transpose the matrix as they both use different majors
      shaderPtr->setUniformValue(shaderUniforms[matrixId], qmat.transposed());
    }
  }
}

const char* MaterialPBR::shaderFileName() const
{
  return "shaderPrograms/redPBR.json";
}



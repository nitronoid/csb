#include "MaterialPBR.h"
#include "Scene.h"
#include "ShaderLib.h"

void MaterialPBR::init(ShaderLib *io_shaderLib, const size_t _index, std::array<glm::mat4, 3>* io_matrices)
{
  Material::init(io_shaderLib, _index, io_matrices);
  auto shaderPtr = m_shaderLib->getCurrentShader();

  shaderPtr->setUniformValue("albedo", 0.5f, 0.0f, 0.0f);
  shaderPtr->setUniformValue("ao", 1.0f);
  shaderPtr->setUniformValue("exposure", 1.0f);
  shaderPtr->setUniformValue("roughness", 0.5f);
  shaderPtr->setUniformValue("metallic", 1.0f);

  // Update our matrices
  update();
}

void MaterialPBR::update()
{
  auto shaderPtr = m_shaderLib->getShader(m_shaderIndex);
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

const char* MaterialPBR::vertexName() const
{
  return "shaders/PBRVertex.glsl";
}

const char* MaterialPBR::fragName() const
{
  return "shaders/PBRFragment.glsl";
}


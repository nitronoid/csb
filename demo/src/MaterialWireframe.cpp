#include "MaterialWireframe.h"
#include "Scene.h"
#include "ShaderLib.h"

void MaterialWireframe::init()
{
  update();
}

void MaterialWireframe::update()
{
  auto shaderPtr = m_shaderLib->getShader(m_shaderName);
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

const char* MaterialWireframe::shaderFileName() const
{
  return "shaderPrograms/wireframe.json";
}

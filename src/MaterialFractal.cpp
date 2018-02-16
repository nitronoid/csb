#include "MaterialFractal.h"
#include "Scene.h"
#include "ShaderLib.h"
#include <chrono>
#include <QOpenGLFunctions>

void MaterialFractal::init()
{
  update();
}

void MaterialFractal::update()
{
  auto shaderPtr = m_shaderLib->getShader(m_shaderName);

  float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock::now() - m_start).count();
  shaderPtr->setUniformValue("t", elapsed / 500.0f);
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

const char* MaterialFractal::shaderFileName() const
{
  return "shaderPrograms/fractal.json";
}

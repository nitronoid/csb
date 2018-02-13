#include "MaterialPhong.h"
#include "Scene.h"
#include "ShaderLib.h"

void MaterialPhong::init(ShaderLib *io_shaderLib, const size_t _index, std::array<glm::mat4, 3>* io_matrices)
{
  Material::init(io_shaderLib, _index, io_matrices);

  //io_shader->setUniform("color", glm::vec3(1.0f, 1.0f, 1.0f));
  // Update our matrices
  update();
}

void MaterialPhong::update()
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

const char* MaterialPhong::vertexName() const
{
  return "shaders/phong_vert.glsl";
}

const char* MaterialPhong::fragName() const
{
  return "shaders/phong_frag.glsl";
}

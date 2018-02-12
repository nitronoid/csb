#include "MaterialPhong.h"
#include "ShaderProgram.h"
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

const char* MaterialPhong::vertexName() const
{
  return "shaders/phong_vert.glsl";
}

const char* MaterialPhong::fragName() const
{
  return "shaders/phong_frag.glsl";
}

#include "MaterialFractal.h"
#include "Scene.h"
#include "ShaderLib.h"
#include <chrono>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_1_Core>

void MaterialFractal::init()
{
  m_last = std::chrono::high_resolution_clock::now();
  update();
}

void MaterialFractal::update()
{
  auto shaderPtr = m_shaderLib->getShader(m_shaderName);

  using namespace std::chrono;
  auto now = high_resolution_clock::now();
  m_time += (duration_cast<milliseconds>(now - m_last).count() * m_updateTime);
  m_last = now;
  shaderPtr->setUniformValue("t", m_time / 1000.0f);
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

void MaterialFractal::handleKey(QKeyEvent* io_event, QOpenGLContext* io_context)
{
  switch(io_event->key())
  {
    case Qt::Key_E :
    {
      static constexpr GLuint id = 1;
      io_context->versionFunctions<QOpenGLFunctions_4_1_Core>()->glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &id);
      break;
    }
    case Qt::Key_R :
    {
      static constexpr GLuint id = 0;
      io_context->versionFunctions<QOpenGLFunctions_4_1_Core>()->glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &id);
      break;
    }
    case Qt::Key_T :
    {
      m_updateTime = !m_updateTime;
      break;
    }
    default: break;
  }

}

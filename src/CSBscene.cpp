#include "CSBscene.h"
#include "MaterialWireframe.h"
#include "MaterialPBR.h"
#include "MaterialCSBpbr.h"
#include "MaterialEnvMap.h"
#include "MaterialFractal.h"
#include <QOpenGLContext>

//-----------------------------------------------------------------------------------------------------
void CSBscene::writeMeshAttributes()
{
  const auto& mesh = m_meshes[m_meshIndex];

  using namespace MeshAttributes;
  for (const auto buff : {VERTEX, UV, NORMAL})
  {
    m_meshVBO.write(mesh.getAttribData(buff), buff);
  }
  m_meshVBO.setIndices(mesh.getIndicesData());
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::setAttributeBuffers()
{
  static constexpr int tupleSize[] = {3,2,3};
  auto prog = m_shaderLib->getCurrentShader();

  using namespace MeshAttributes;
  for (const auto buff : {VERTEX, UV, NORMAL})
  {
    prog->enableAttributeArray(buff);
    prog->setAttributeBuffer(buff, GL_FLOAT, m_meshVBO.offset(buff), tupleSize[buff]);
  }
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::init()
{
  Scene::init();

  initMaterials();
  initGeo();

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    m_matrices[MODEL_VIEW] = glm::translate(m_matrices[MODEL_VIEW], glm::vec3(0.0f, 0.0f, -2.0f));
    m_matrices[MODEL_VIEW] = glm::rotate(m_matrices[MODEL_VIEW], glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  }
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::initGeo()
{
  m_meshes[0].load("models/hdxPlane.obj");
//  m_meshes[0].load("models/hdCube.obj");
  for (auto& mesh : m_meshes) m_solver.addTriangleMesh(mesh);
  // Create and bind our Vertex Array Object
  m_vao->create();
  m_vao->bind();
  // Create and bind our Vertex Buffer Object
  m_meshVBO.init();
  generateNewGeometry();
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::keyPress(QKeyEvent* io_event)
{
  makeCurrent();
  Scene::keyPress(io_event);
  m_materials[m_currentMaterial]->handleKey(io_event, context());
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::initMaterials()
{
  m_materials.reserve(5);

  m_materials.emplace_back(new MaterialWireframe(m_camera, m_shaderLib, &m_matrices));
  m_materials.emplace_back(new MaterialEnvMap(m_camera, m_shaderLib, &m_matrices));
  m_materials.emplace_back(new MaterialCSBpbr(m_camera, m_shaderLib, &m_matrices, {0.5f, 0.0f, 0.0f}, 1.0f, 1.0f, 0.5f, 1.0f));
  m_materials.emplace_back(new MaterialFractal(m_camera, m_shaderLib, &m_matrices));
  for (size_t i = 0; i < m_materials.size(); ++i)
  {
    auto& mat = m_materials[i];
    auto name = m_shaderLib->loadShaderProg(mat->shaderFileName());
    mat->setShaderName(name);
    mat->apply();
  }
  m_materials[m_currentMaterial]->apply();
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::rotating( const bool _rotating )
{
  m_rotating = _rotating;
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::generateNewGeometry()
{
  makeCurrent();
  m_meshIndex = (m_meshIndex + 1) % m_meshes.size();
  auto& mesh = m_meshes[m_meshIndex];
  m_meshVBO.reset(
        sizeof(GLushort),
        mesh.getNIndicesData(),
        sizeof(GLfloat),
        mesh.getNVertData(),
        mesh.getNUVData(),
        mesh.getNNormData()
        );
  writeMeshAttributes();
  setAttributeBuffers();
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::nextMaterial()
{
  makeCurrent();
  m_currentMaterial = (m_currentMaterial + 1) % m_materials.size();

  m_materials[m_currentMaterial]->apply();
  setAttributeBuffers();
}
//-----------------------------------------------------------------------------------------------------
void CSBscene::renderScene()
{
  Scene::renderScene();

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    m_matrices[MODEL_VIEW] = glm::rotate(m_matrices[MODEL_VIEW], glm::radians(-1.0f * m_rotating), glm::vec3(0.0f, 1.0f, 0.0f));
  }

  m_materials[m_currentMaterial]->update();
  using namespace std::chrono;

  static constexpr float dt = 1.f/30.f;
  static float accum = 0.0f;
  static auto currentTime = high_resolution_clock::now();

  const auto time = high_resolution_clock::now();

  float ft = duration_cast<milliseconds>(time - currentTime).count() / 1000.0f;
//  ft = std::min(0.05f, ft);
  currentTime = time;
  accum += ft;

  while(accum >= dt)
  {
    m_solver.update(dt);
//    m_meshes[m_meshIndex].update(dt);
    accum -=dt;
  }

  writeMeshAttributes();

  glDrawElements(GL_TRIANGLES, m_meshes[m_meshIndex].getNIndicesData(), GL_UNSIGNED_SHORT, nullptr);
}
//-----------------------------------------------------------------------------------------------------

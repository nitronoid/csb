#include "CSBscene.h"
#include "MaterialWireframe.h"
#include "MaterialPBR.h"
#include "MaterialCSBpbr.h"
#include "MaterialEnvMap.h"
#include "MaterialFractal.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_0_Core>

//-----------------------------------------------------------------------------------------------------
void CSBscene::writeMeshAttributes()
{
  using namespace MeshAttributes;
  for (size_t i = 0; i < m_meshes.size(); ++i)
  {
    const auto& mesh = m_meshes[i];
    for (const auto buff : {VERTEX, UV, NORMAL})
    {
      m_meshVBO.write(mesh.getAttribData(buff), buff, mesh.getNAttribData(buff), m_meshOffsets[i][buff]);
    }
    m_meshVBO.writeIndices(mesh.getIndicesData(), mesh.getNIndices(), m_meshIndexStartPoints[i]);
  }
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
  size_t num = 2;
  m_meshes.resize(num);
  m_meshOffsets.resize(num);
  m_meshIndexStartPoints.resize(num);
  m_meshes[0].load("models/Sphere.obj");
  m_meshes[1].load("models/cube.obj");

  m_meshIndexStartPoints[0] = 0;
  m_meshOffsets[0] = {{0,0,0}};
  for (size_t i = 1; i < num; ++i)
  {
    using namespace MeshAttributes;
    for (const auto buff : {VERTEX, UV, NORMAL})
      m_meshOffsets[i][buff] = m_meshes[i-1].getNAttribData(buff);
    m_meshIndexStartPoints[i] = m_meshes[i-1].getNIndices();
  }

  auto totalIndices = 0;
  auto totalVerts = 0;
  auto totalUVs = 0;
  auto totalNorms = 0;

  for (auto& mesh : m_meshes)
  {
    mesh.init();
    m_solver.addTriangleMesh(mesh);

    totalIndices += mesh.getNIndicesData();
    totalVerts += mesh.getNVertData();
    totalUVs += mesh.getNUVData();
    totalNorms +=  mesh.getNNormData();
    m_numIndices.push_back(mesh.getNIndices());
  }
  // Create and bind our Vertex Array Object
  m_vao->create();
  m_vao->bind();
  // Create and bind our Vertex Buffer Object
  m_meshVBO.init();
  m_numIndex = totalIndices;

  m_meshVBO.reset(
        sizeof(GLushort),
        totalIndices,
        sizeof(GLfloat),
        totalVerts,
        totalUVs,
        totalNorms
        );

  writeMeshAttributes();
  setAttributeBuffers();
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
//  using namespace std::chrono;

//  static constexpr float dt = 1.f/30.f;
//  static float accum = 0.0f;
//  static auto currentTime = high_resolution_clock::now();

//  const auto time = high_resolution_clock::now();

//  float ft = duration_cast<milliseconds>(time - currentTime).count() / 1000.0f;
////  ft = std::min(0.05f, ft);
//  currentTime = time;
//  accum += ft;

//  while(accum >= dt)
//  {
//    m_solver.update(dt);
//    accum -=dt;
//  }

//  writeMeshAttributes();

  glDrawElements(GL_TRIANGLES, m_numIndex, GL_UNSIGNED_SHORT, nullptr);
}
//-----------------------------------------------------------------------------------------------------

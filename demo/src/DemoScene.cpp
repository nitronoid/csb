#include "DemoScene.h"
#include "MaterialWireframe.h"
#include "MaterialCSBpbr.h"
#include "MaterialFractal.h"
#include "SphereCollisionConstraint.h"
#include "SelfCollisionSpheresConstraint.h"
#include "SelfCollisionRaysConstraint.h"
#include <QOpenGLContext>

//-----------------------------------------------------------------------------------------------------
void DemoScene::writeMeshAttributes()
{
  using namespace csb;
  for (size_t i = 0; i < m_meshes.size(); ++i)
  {
    const auto& mesh = m_meshes[i];
    for (const auto buff : {VERTEX, UV, NORMAL})
    {
      m_meshVBO.write(mesh->getAttribData(buff), buff, mesh->getNAttribData(buff), m_meshAttributeOffsets[i][buff]);
    }
    m_meshVBO.writeIndices(mesh->getIndicesData(), mesh->getNIndicesData(), m_meshIndexStartPoints[i]);
  }
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::setAttributeBuffers()
{
  static constexpr int tupleSize[] = {3,2,3};
  auto prog = m_shaderLib->getCurrentShader();

  using namespace csb;
  for (const auto buff : {VERTEX, UV, NORMAL})
  {
    prog->enableAttributeArray(buff);
    prog->setAttributeBuffer(buff, GL_FLOAT, m_meshVBO.offset(buff), tupleSize[buff]);
  }
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::init()
{
  Scene::init();

  m_qogl_funcs = context()->versionFunctions<QOpenGLFunctions_4_1_Core>();

  initMaterials();
  initSimMeshes();
  prepMeshesGL();
  initSolver();

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    m_matrices[MODEL_VIEW] = glm::translate(m_matrices[MODEL_VIEW], glm::vec3(0.0f, 0.0f, -2.0f));
    m_matrices[MODEL_VIEW] = glm::rotate(m_matrices[MODEL_VIEW], glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  }
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::initSimMeshes()
{
  m_meshes.emplace_back(std::make_shared<csb::SimulatedMesh>());
  // Load some meshes and apply constraints
  m_meshes[0]->load("models/uhdPlane.obj");
  m_meshes[0]->init();
  m_meshes[0]->setParticleInverseMass(0, 0.f);
  m_meshes[0]->setParticleInverseMass(90, 0.f);
//  m_meshes[0]->setParticleInverseMass(m_meshes[0]->getNVerts() - 3, 0.f);
//  m_meshes[0]->generateStructuralConstraints();
  m_meshes[0]->generateClothConstraints(0.05f);
  m_solver.addSimulatedMesh(m_meshes[0]);


  m_meshes.emplace_back(std::make_shared<csb::SimulatedMesh>());
  m_meshes[1]->load("models/Sphere.obj");
  m_meshes[1]->translate({-0.1f, -0.8f, 0.f});
  m_solver.addStaticCollision(new csb::SphereCollisionConstraint({-0.1f,-0.8f,0.f}, 0.455f));
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::initSolver()
{
  m_solver.addContinuousCollision(new csb::SelfCollisionRaysConstraint);
  m_solver.addContinuousCollision(new csb::SelfCollisionSpheresConstraint(m_meshes[0]->getAverageEdgeLength()));
  m_solver.addForce({0.f, -5.f, 0.f});
  m_solver.setDamping(0.1f);
  m_solver.setPositionConstraintIterations(30);
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::prepMeshesGL()
{
  // Resize all of our offset counters to the same size
  const auto size = m_meshes.size();
  m_meshAttributeOffsets.resize(size);
  m_meshIndexStartPoints.resize(size);
  m_meshIndexOffsets.resize(size);
  m_meshBaseVert.resize(size);
  m_numIndicesPerMesh.reserve(size);

  m_meshIndexStartPoints[0] = 0;
  m_meshAttributeOffsets[0] = {{0,0,0}};
  m_meshIndexOffsets[0] = static_cast<char*>(nullptr);
  for (size_t i = 1; i < size; ++i)
  {
    using namespace csb;
    for (const auto buff : {VERTEX, UV, NORMAL})
      m_meshAttributeOffsets[i][buff] = m_meshes[i-1]->getNAttribData(buff);
    m_meshBaseVert[i] = static_cast<GLint>(m_meshes[i-1]->getNVerts());
    m_meshIndexStartPoints[i] = static_cast<GLsizei>(m_meshes[i-1]->getNIndices());
    m_meshIndexOffsets[i] = static_cast<char*>(nullptr) + m_meshes[i-1]->getNIndices() * sizeof(GLushort);
  }


  int totalIndices = 0, totalVerts = 0, totalUVs = 0, totalNorms = 0;

  for (auto& mesh : m_meshes)
  {
    totalIndices += mesh->getNIndicesData();
    totalVerts += mesh->getNVertData();
    totalUVs += mesh->getNUVData();
    totalNorms +=  mesh->getNNormData();
    m_numIndicesPerMesh.push_back(static_cast<GLsizei>(mesh->getNIndices()));
  }
  // Create and bind our Vertex Array Object
  m_vao->create();
  m_vao->bind();
  // Create and bind our Vertex Buffer Object
  m_meshVBO.init();

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
void DemoScene::keyPress(QKeyEvent* io_event)
{
  makeCurrent();
  Scene::keyPress(io_event);
  m_materials[m_currentMaterial]->handleKey(io_event, context());
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::initMaterials()
{
  m_materials.reserve(5);

  m_materials.emplace_back(new MaterialCSBpbr(m_camera, m_shaderLib, &m_matrices, {0.5f, 0.0f, 0.0f}, 1.0f, 1.0f, 0.5f, 1.0f));
  m_materials.emplace_back(new MaterialWireframe(m_camera, m_shaderLib, &m_matrices));
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
void DemoScene::rotating( const bool _rotating )
{
  m_rotating = _rotating;
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::nextMaterial()
{
  makeCurrent();
  m_currentMaterial = (m_currentMaterial + 1) % m_materials.size();

  m_materials[m_currentMaterial]->apply();
  setAttributeBuffers();
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::renderScene()
{
  Scene::renderScene();

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    m_matrices[MODEL_VIEW] = glm::rotate(m_matrices[MODEL_VIEW], glm::radians(-1.0f * m_rotating), glm::vec3(0.0f, 1.0f, 0.0f));
  }

  m_materials[m_currentMaterial]->update();

  m_solver.update();

  writeMeshAttributes();
  m_qogl_funcs->glMultiDrawElementsBaseVertex(
        GL_TRIANGLES,
        m_numIndicesPerMesh.data(),
        GL_UNSIGNED_SHORT,
        m_meshIndexOffsets.data(),
        static_cast<GLsizei>(m_meshes.size()),
        m_meshBaseVert.data()
        );
}
//-----------------------------------------------------------------------------------------------------

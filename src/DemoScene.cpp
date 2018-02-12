#include "DemoScene.h"


//-----------------------------------------------------------------------------------------------------
void DemoScene::loadMesh()
{
  static constexpr std::array<const char*, 3> shaderAttribs = {{"inVert", "inNormal", "inUV"}};
  const auto& mesh = m_meshes[m_meshIndex];
  const std::vector<const float*> meshData {
    mesh.getVertexData(), mesh.getNormalsData(), mesh.getUVsData()
  };

  using b = Buffer::BufferType;
  for (const auto buff : {b::VERTEX, b::NORMAL, b::UV})
  {
    m_buffer.append(meshData[buff], buff);
    GLuint pos = static_cast<GLuint>(glGetAttribLocation(m_shaderLib->getShader(m_currentMaterial)->getShaderProgram(), shaderAttribs[buff]));
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  }
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::init()
{
  Scene::init();

  m_meshes[0].load("models/cube.obj");
  m_meshes[1].load("models/Face.obj");
  m_meshes[2].load("models/Suzanne.obj");
  m_meshes[3].load("models/test2.obj");
  m_meshes[4].load("models/Asteroid.obj");
  m_rotating = false;

  m_shaderLib->m_shaders.resize(2);
  for (size_t i = 0; i < m_materials.size(); ++i)
  {
    auto& mat = m_materials[i];
//    m_shaderLib->m_shaders.emplace_back();
    m_shaderLib->createShader(mat->vertexName(), mat->fragName());
    m_shaderLib->getShader(i)->init(mat->vertexName(), mat->fragName());
    m_shaderLib->useShader(i);
    mat->init(m_shaderLib, i, &m_matrices);
  }
  m_materials[m_currentMaterial]->apply();
  m_buffer.init(sizeof(float), m_meshes[m_meshIndex].getNVertData());
  loadMesh();

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    m_matrices[MODEL_VIEW] = glm::translate(m_matrices[MODEL_VIEW], glm::vec3(0.0f, 0.0f, -2.0f));
  }
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::rotating( const bool _rotating )
{
  m_rotating = _rotating;
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::generateNewGeometry()
{
  m_meshIndex = (m_meshIndex + 1) % m_meshes.size();
  m_buffer.reset(sizeof(float), m_meshes[m_meshIndex].getNVertData());
  loadMesh();
}
//-----------------------------------------------------------------------------------------------------
void DemoScene::nextMaterial()
{
  makeCurrent();
  m_currentMaterial = (m_currentMaterial + 1) % m_materials.size();
  m_materials[m_currentMaterial]->apply();
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

  glDrawArrays(GL_TRIANGLES, 0, m_buffer.dataAmount() / 3);
}
//-----------------------------------------------------------------------------------------------------

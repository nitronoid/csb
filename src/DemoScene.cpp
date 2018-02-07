#include "demoscene.h"

void DemoScene::loadMesh()
{
  static constexpr std::array<const char*, 3> shaderAttribs = {{"inVert", "inNormal", "inUV"}};
  const std::vector<const float*> meshData {
    m_mesh->getVertexData(), m_mesh->getNormalsData(), m_mesh->getUVsData()
  };

  using b = Buffer::BufferType;
  for (const auto buff : {b::VERTEX, b::NORMAL, b::UV})
  {
    m_buffer.append(meshData[buff], sizeof(float), buff);
    GLuint pos = static_cast<GLuint>(glGetAttribLocation(m_shaderProgram.getShaderProgram(), shaderAttribs[buff]));
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  }
}

void DemoScene::init()
{
  Scene::init();

  m_meshes[0].load("models/cube.obj");
  m_meshes[1].load("models/Face.obj");
  m_meshes[2].load("models/Suzanne.obj");
  m_meshes[3].load("models/test2.obj");
  m_meshes[4].load("models/Asteroid.obj");
  m_mesh = &m_meshes[0];
  //m_camera->setAspectRatio(static_cast<float>(width())/static_cast<float>(height()));
  m_rotating = false;

  std::string shadersAddress = "shaders/";
  m_shaderProgram.init("m_shader", shadersAddress + "PBRVertex.glsl", shadersAddress + "PBRFragment.glsl");
  m_shaderProgram.use();
  m_material->init(&m_shaderProgram);
  m_buffer.init(sizeof(float), static_cast<GLuint>(m_mesh->getNVertData()));
  loadMesh();

  auto modelView = m_material->modelViewMatrix();
  *modelView = glm::translate(*modelView, glm::vec3(0.0f, 0.0f, -2.0f));
}

void DemoScene::generateNewGeometry()
{
  static size_t count = 0;
  count = (count + 1) % m_meshes.size();
  m_mesh = &m_meshes[count];
  m_buffer.reset(sizeof(float), static_cast<GLuint>(m_mesh->getNVertData()));
  loadMesh();
}

void DemoScene::renderScene()
{
  Scene::renderScene();

  auto modelView = m_material->modelViewMatrix();
  *modelView = glm::rotate(*modelView, glm::radians(-1.0f * m_rotating), glm::vec3(0.0f, 1.0f, 0.0f));

  m_material->update();

  glDrawArrays(GL_TRIANGLES, 0, m_buffer.dataSize() / 3);
}

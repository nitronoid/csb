#include "Scene.h"
#include <iostream>
#include <QColorDialog>
#include <QGLWidget>
#include <QImage>
#include <QScreen>
#include "MaterialPBR.h"

//----------------------------------------------------------------------------------------------------------------------
Scene::Scene(Camera* io_camera , QWidget *io_parent) :
  QOpenGLWidget(io_parent),
  m_camera(io_camera)
{
  // set this widget to have the initial keyboard focus
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize( io_parent->size() );
}
//----------------------------------------------------------------------------------------------------------------------
void Scene::initializeGL()
{
#ifdef linux
  // this needs to be after the context creation, otherwise GLEW will crash
  //std::cout <<"linux \n";
  glewExperimental = GL_TRUE;
  glewInit();
  //	GLenum error = glGetError();
#endif
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_MULTISAMPLE );
  glEnable( GL_TEXTURE_2D );
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glViewport(0, 0, width()*devicePixelRatio(), height()*devicePixelRatio());

  // This calls the derived class init func
  init();
}
//----------------------------------------------------------------------------------------------------------------------
void Scene::resizeGL( int _w, int _h )
{
  m_camera->resize(_w, _h);
}
//----------------------------------------------------------------------------------------------------------------------
void Scene::keyPress(QKeyEvent* _event)
{
  m_camera->handleKey(_event->key(), (_event->type()==QKeyEvent::KeyPress));
}
//----------------------------------------------------------------------------------------------------------------------
void Scene::mouseMove(QMouseEvent * _event)
{
  m_camera->handleMouseMove(glm::vec2{_event->pos().x(), _event->pos().y()});
  update();
}
//----------------------------------------------------------------------------------------------------------------------
void Scene::mouseClick(QMouseEvent * _event)
{
  m_camera->handleMouseClick(*_event);
  update();
}
//----------------------------------------------------------------------------------------------------------------------
void Scene::init()
{
  m_camera->setMousePos(0,0);
}
//------------------------------------------------------------------------------------------------------------------------------
void Scene::paintGL()
{
  // Set our viewport size based on the devide displace
  glViewport(0, 0, width()*devicePixelRatio(), height()*devicePixelRatio());
  // Call the render scene function that may be overriden
  renderScene();
  // Update the OpenGL widget
  update();
}
//------------------------------------------------------------------------------------------------------------------------------
void Scene::renderScene()
{
  // Clear the screen
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Update our camera view
  m_camera->update();

  // Scope the using declaration
  {
    using namespace SceneMatrices;
    m_matrices[PROJECTION] = m_camera->projMatrix() * m_camera->viewMatrix() * m_matrices[MODEL_VIEW];
    m_matrices[NORMAL] = glm::inverse(glm::transpose(m_matrices[MODEL_VIEW]));
  }
}
//------------------------------------------------------------------------------------------------------------------------------



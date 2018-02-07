#include "Scene.h"
#include <iostream>
#include <QColorDialog>
#include <QGLWidget>
#include <QImage>
#include <QScreen>
#include "MaterialPBR.h"

//----------------------------------------------------------------------------------------------------------------------
Scene::Scene(Camera* io_camera , QWidget *_parent) :
  QOpenGLWidget(_parent),
  m_camera(io_camera)
{
  // set this widget to have the initial keyboard focus
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize( _parent->size() );
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
  glViewport(0, 0, width()*devicePixelRatio(), height()*devicePixelRatio());
  glClearColor( 1, 1, 1, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  renderScene();
  update();
}
//------------------------------------------------------------------------------------------------------------------------------
void Scene::renderScene()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_camera->update();
}
//------------------------------------------------------------------------------------------------------------------------------



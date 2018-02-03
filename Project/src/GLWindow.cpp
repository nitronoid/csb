#include "GLWindow.h"

#include <iostream>
#include <QColorDialog>
#include <QGLWidget>
#include <QImage>
#include <QScreen>
//----------------------------------------------------------------------------------------------------------------------

GLWindow::GLWindow( QWidget *_parent ) : QOpenGLWidget( _parent )
{
  // set this widget to have the initial keyboard focus
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize( _parent->size() );
  m_camera.setMousePos(0,0);
  m_camera.setTarget(0.0f, 0.0f, -2.0f);
  m_camera.setEye(0.0f, 0.0f, 0.0f);
  m_rotating = false;

}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::initializeGL()
{
#ifdef linux
  // this needs to be after the context creation, otherwise it GLEW will crash
  //std::cout <<"linux \n";
  glewExperimental = GL_TRUE;
  glewInit();
  //	GLenum error = glGetError();
#endif
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_MULTISAMPLE );
  glEnable( GL_TEXTURE_2D );
  glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
  glViewport( 0, 0, devicePixelRatio(), devicePixelRatio() );

  m_meshes[0] = Mesh( "models/cube.obj", "cube" );
  m_meshes[1] = Mesh( "models/Face.obj", "Face" );
  m_meshes[2] = Mesh( "models/Suzanne.obj", "Suzanne" );
  m_meshes[3] = Mesh( "models/test2.obj", "weirdShape" );
  m_meshes[4] = Mesh( "models/Asteroid.obj", "Asteroid" );
  m_mesh = & m_meshes[0];

  init();
  m_MV = glm::translate( m_MV, glm::vec3(0.0f, 0.0f, -2.0f) );
}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::resizeGL( int _w, int _h )
{

}

//----------------------------------------------------------------------------------------------------------------------

GLWindow::~GLWindow()
{

}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::mouseMove(QMouseEvent * _event)
{
  m_camera.handleMouseMove( _event->pos().x(), _event->pos().y() );

  update();
}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::mouseClick(QMouseEvent * _event)
{
  m_camera.handleMouseClick(*_event);

  update();
}

void GLWindow::loadMesh()
{
  m_mesh->setBufferIndex(0);
  m_amountVertexData = static_cast<GLsizeiptr>(m_mesh->getAmountVertexData() * sizeof(float));

  // load vertices
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, m_amountVertexData, nullptr, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(m_mesh->getAmountVertexData() * sizeof(float)), &m_mesh->getVertexData());

  // pass vertices to shader
  GLuint pos = static_cast<GLuint>(glGetAttribLocation(m_shader.getShaderProgram(), "VertexPosition"));
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // load normals
  glBindBuffer(GL_ARRAY_BUFFER,	m_nbo );
  glBufferData(GL_ARRAY_BUFFER, m_amountVertexData, nullptr, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(m_mesh->getAmountVertexData() * sizeof(float)), &m_mesh->getNormalsData());

  // pass normals to shader
  GLuint n = static_cast<GLuint>(glGetAttribLocation(m_shader.getShaderProgram(), "VertexNormal"));
  glEnableVertexAttribArray(n);
  glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::init()
{
  std::string shadersAddress = "shaders/";
  m_shader = Shader("m_shader", shadersAddress + "phong_vert.glsl", shadersAddress + "simplefrag.glsl");

  glLinkProgram(m_shader.getShaderProgram());
  glUseProgram(m_shader.getShaderProgram());

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_nbo);

  loadMesh();

  // link matrices with shader locations
  m_MVAddress = glGetUniformLocation( m_shader.getShaderProgram(), "MV" );
  m_MVPAddress = glGetUniformLocation( m_shader.getShaderProgram(), "MVP" );
  m_NAddress = glGetUniformLocation( m_shader.getShaderProgram(), "N" );
}

//------------------------------------------------------------------------------------------------------------------------------

void GLWindow::paintGL()
{
  glViewport( 0, 0, width(), height() );
  glClearColor( 1, 1, 1, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  renderScene();

  update();
}

//------------------------------------------------------------------------------------------------------------------------------

void GLWindow::renderScene()
{
  glViewport( 0, 0, width()*devicePixelRatio(), height()*devicePixelRatio() ); //fix for retina screens
  glClearColor( 1, 1, 1, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  m_camera.update();
  m_projection = glm::perspective( glm::radians( 60.0f ),
                                   static_cast<float>( width() ) / static_cast<float>( height() ), 0.1f, 100.0f );
  m_view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 5.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

  if ( m_rotating )
    m_MV = glm::rotate( m_MV, glm::radians( -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  m_MVP = m_projection * m_camera.viewMatrix() * m_MV;
  glm::mat3 N = glm::mat3( glm::inverse( glm::transpose( m_MV ) ) );

  glUniformMatrix4fv(m_MVPAddress, 1, GL_FALSE, glm::value_ptr(m_MVP));
  glUniformMatrix4fv(m_MVAddress, 1, GL_FALSE, glm::value_ptr(m_MV));

  glUniformMatrix3fv(m_NAddress, 1, GL_FALSE, glm::value_ptr( N));

  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_amountVertexData) / 12);
}

//------------------------------------------------------------------------------------------------------------------------------

void GLWindow::generateNewGeometry()
{
  static size_t count = 0;
  count = (count + 1) % m_meshes.size();
  m_mesh = &m_meshes[count];
  loadMesh();
}

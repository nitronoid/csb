#include "Scene.h"

Scene::Scene()
{
  m_camera.setInitialMousePos(0,0);
  m_camera.setTarget(0.0f, 0.0f, -2.0f);
  m_camera.setEye(0.0f, 0.0f, 0.0f);
//  m_buffer = Buffer();
//  init();
  m_MV = glm::translate( m_MV, glm::vec3(0.0f, 0.0f, -2.0f) );
}

//----------------------------------------------------------------------------------------------------------------------

Scene::~Scene()
{

}

//----------------------------------------------------------------------------------------------------------------------

void Scene::mouseClick(QMouseEvent * _event )
{
  m_camera.handleMouseClick(_event->pos().x(), _event->pos().y(), _event->type(), _event, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void Scene::mouseMove(QMouseEvent * _event)
{
  m_camera.handleMouseMove( _event->pos().x(), _event->pos().y() );
}

//----------------------------------------------------------------------------------------------------------------------

void Scene::init()
{

}

//----------------------------------------------------------------------------------------------------------------------

void Scene::createMesh()
{
}

//----------------------------------------------------------------------------------------------------------------------

void Scene::Update()
{
    glViewport( 0, 0, m_width, m_height );
    glClearColor( 1, 1, 1, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram( m_shader.getShaderProgram() );

    glBindVertexArray( m_vao );
    glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
    glEnableVertexAttribArray( glGetAttribLocation( m_shader.getShaderProgram(), "VertexPosition" ) );
    glBindBuffer( GL_ARRAY_BUFFER, m_nbo );
    glEnableVertexAttribArray( glGetAttribLocation( m_shader.getShaderProgram(), "VertexNormal" ) );

    m_camera.update();
    m_projection = glm::perspective( glm::radians( 60.0f ),
    static_cast<float>( m_width ) / static_cast<float>( m_height ), 0.1f, 100.0f );
    m_view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 5.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

    m_MVP = m_projection * m_camera.viewMatrix() * m_MV;
    glm::mat3 N = glm::mat3( glm::inverse( glm::transpose( m_MV ) ) );

    glUniformMatrix4fv( m_MVPAddress, 1, GL_FALSE, glm::value_ptr( m_MVP ) );
    glUniformMatrix4fv( m_MVAddress, 1, GL_FALSE, glm::value_ptr( m_MV ) );

    glUniformMatrix3fv( m_NAddress, 1, GL_FALSE, glm::value_ptr( N ) );

    glDrawArrays( GL_TRIANGLES, m_mesh.getBufferIndex() / 3, ( m_mesh.getAmountVertexData() / 3 ) );
}

//----------------------------------------------------------------------------------------------------------------------

void Scene::Draw() const
{
  glViewport( 0, 0, m_width, m_height );
  glClearColor( 1, 1, 1, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glDrawArrays( GL_TRIANGLES, m_mesh.getBufferIndex() / 3, ( m_mesh.getAmountVertexData() / 3 ) );
}

//----------------------------------------------------------------------------------------------------------------------

void Scene::setSize(const int _width, const int _height)
{
  m_width = _width;
  m_height = _height;
}

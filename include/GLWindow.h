#ifndef NGLSCENE_H_
#define NGLSCENE_H_


#include "ShaderProgram.h"
#include "Mesh.h"
#include <gtc/matrix_transform.hpp>
#include <ext.hpp>
#include <glm.hpp>
#include <QOpenGLWidget>
#include <QResizeEvent>
#include <QEvent>
#include <memory>
#include "Buffer.h"
#include "Camera.h"

class Material;

class GLWindow : public QOpenGLWidget
{
  Q_OBJECT        // must include this if you use Qt signals/slots
public :
  /// @brief Constructor for GLWindow
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for GLWindow
  /// @param [in] _parent the parent window to create the GL context in
  //----------------------------------------------------------------------------------------------------------------------
  GLWindow(Camera* io_camera, QWidget *_parent);

  /// @brief dtor
  ~GLWindow();
  void mouseMove( QMouseEvent * _event );
  void mouseClick( QMouseEvent * _event );

public slots:
  void rotating( const bool _rotating ) { m_rotating = _rotating; }
  void init();
  void generateNewGeometry();
protected:
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL();

  void loadMesh();
  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  void resizeGL(int _w , int _h);
  /// @brief this is the main gl drawing routine which is called whenever the window needs to be re-drawn
  void paintGL();
  void renderScene();

private :
  //----------------------------------------------------------------------------------------------------------------------
  AMesh* m_mesh;
  //----------------------------------------------------------------------------------------------------------------------
  std::array<AMesh, 5> m_meshes;
  //----------------------------------------------------------------------------------------------------------------------
  ShaderProgram m_shaderProgram;
  //----------------------------------------------------------------------------------------------------------------------
  std::unique_ptr<Material> m_material;
  //----------------------------------------------------------------------------------------------------------------------
  Camera* m_camera;
  //----------------------------------------------------------------------------------------------------------------------
  bool m_rotating;
  //----------------------------------------------------------------------------------------------------------------------
  Buffer m_buffer;
};

#endif

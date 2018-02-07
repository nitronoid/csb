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

class Scene : public QOpenGLWidget
{
  Q_OBJECT        // must include this if you use Qt signals/slots
public :
  /// @brief Constructor for GLWindow
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for GLWindow
  /// @param [in] _parent the parent window to create the GL context in
  //----------------------------------------------------------------------------------------------------------------------
  Scene(Camera* io_camera, QWidget *_parent);

  Scene(const Scene&) = default;
  Scene& operator=(const Scene&) = default;
  Scene(Scene&&) = default;
  Scene& operator=(Scene&&) = default;
  /// @brief dtor
  virtual ~Scene() = default;

  void mouseMove( QMouseEvent * _event );
  void mouseClick( QMouseEvent * _event );

protected:
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL();
  virtual void init();
  void loadMesh();
  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  void resizeGL(int _w , int _h);
  /// @brief this is the main gl drawing routine which is called whenever the window needs to be re-drawn
  void paintGL();
  virtual void renderScene() = 0;

private:
  //----------------------------------------------------------------------------------------------------------------------
  Camera* m_camera;
};

#endif

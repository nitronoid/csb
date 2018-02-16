#ifndef SCENE_H_
#define SCENE_H_

#include "Mesh.h"
#include <gtc/matrix_transform.hpp>
#include <ext.hpp>
#include <glm.hpp>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QResizeEvent>
#include <QEvent>
#include <memory>
#include "MeshVBO.h"
#include "Camera.h"


//-------------------------------------------------------------------------------------------------------
/// @brief Enum used to access matrices in a more readable way.
//-------------------------------------------------------------------------------------------------------
namespace SceneMatrices
{
enum MATRIX { MODEL_VIEW, PROJECTION, NORMAL };
}

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor for Scene.
  /// @param [io] io_camera the camera used to view the scene.
  /// @param [io] io_parent the parent window to create the GL context in.
  //-----------------------------------------------------------------------------------------------------
  Scene(const std::shared_ptr<Camera> &io_camera, QWidget *io_parent);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  Scene(const Scene&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Scene& operator=(const Scene&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  Scene(Scene&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Scene& operator=(Scene&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default virtual destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~Scene() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Receives and acts on a key event.
  /// @param [io] io_event is the key event that was received.
  //-----------------------------------------------------------------------------------------------------
  virtual void keyPress(QKeyEvent* io_event);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Receives and acts on a mouse event, when moved.
  /// @param [io] io_event is the mouse event that was received.
  //-----------------------------------------------------------------------------------------------------
  virtual void mouseMove(QMouseEvent * io_event);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Receives and acts on a mouse event, when clicked.
  /// @param [io] io_event is the mouse event that was received.
  //-----------------------------------------------------------------------------------------------------
  virtual void mouseClick(QMouseEvent * io_event);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to intialise the scene, subclasses must call this base function.
  //-----------------------------------------------------------------------------------------------------
  virtual void init();

protected:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to intialise the OpenGL context and apply our settings.
  //-----------------------------------------------------------------------------------------------------
  void initializeGL();
  //-----------------------------------------------------------------------------------------------------
  /// @brief this is called whenever the window is re-sized
  /// @param [in] _w the width of the resized window
  /// @param [in] _h the height of the resized window
  //-----------------------------------------------------------------------------------------------------
  void resizeGL(int _w , int _h);
  //-----------------------------------------------------------------------------------------------------
  /// @brief this is the main gl drawing routine which is called whenever the window needs to be re-drawn,
  /// you should implement your draw calls in renderScene as this function calls those.
  //-----------------------------------------------------------------------------------------------------
  void paintGL();
  //-----------------------------------------------------------------------------------------------------
  /// @brief you should implement this function to draw your openGL scene, the start of your,
  /// implementation should call this base function.
  //-----------------------------------------------------------------------------------------------------
  virtual void renderScene();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Array of matrices, that stores the model view, projection, and normal matrices.
  //-----------------------------------------------------------------------------------------------------
  std::array<glm::mat4, 3> m_matrices;
  //-----------------------------------------------------------------------------------------------------
  /// @brief a pointer to the camera that is used to view this scene.
  //-----------------------------------------------------------------------------------------------------
  std::shared_ptr<Camera> m_camera;

};

#endif //SCENE_H_

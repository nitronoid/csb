#ifndef CAMERA_H
#define CAMERA_H

///**********************************PLEASE NOTICE*****************************************///
///****************************************************************************************///
///*																					  *///
///*	THIS CLASS BELONGS TO RICHARD SOUTHERN,                                           *///
///*	MINOR CHANGES WERE MADE TO MAKE SURE THE CAMERA WORKED WITH SDL					  *///
///*  BUT IT SHOUDLN'T BE MARKED														  *///
///*	THE ORIGINAL FILE IS FROM A RENDERING WORKSHOP (TRACKBALL - 2/2/2017)             *///
///*																					  *///
///****************************************************************************************///
///****************************************************************************************///

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <chrono>
#include <QMouseEvent>

/**
 * @brief The Camera class
 */
class Camera
{
public:
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  ///-----------------------------------------------------------------------------------------------------
  Camera() = default;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  ///-----------------------------------------------------------------------------------------------------
  Camera(const Camera&) = default;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  ///-----------------------------------------------------------------------------------------------------
  Camera& operator=(const Camera&) = default;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  ///-----------------------------------------------------------------------------------------------------
  Camera(Camera&&) = default;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  ///-----------------------------------------------------------------------------------------------------
  Camera& operator=(Camera&&) = default;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Default virtual destructor.
  ///-----------------------------------------------------------------------------------------------------
  virtual ~Camera() = default;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to resize the openGL view by changing the aspect ratio, used for the projection matrix.
  /// @param _width is the width we have resized to.
  /// @param _height is the height we have resized to.
  ///-----------------------------------------------------------------------------------------------------
  virtual void resize(const int _width, const int _height);
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to update the cameras matrices, call before retrieving them.
  ///-----------------------------------------------------------------------------------------------------
  virtual void update();
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Handles key press events.
  /// @note THIS IS UNUSED AND NEEDS FIXING
  ///-----------------------------------------------------------------------------------------------------
  virtual void handleKey(const int _glfwKey, const bool _isPress) = 0;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to respond to mouse movement.
  /// @param _mousePos is the new position of the mouse.
  ///-----------------------------------------------------------------------------------------------------
  virtual void handleMouseMove(const glm::vec2 &_mousePos) = 0;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to respond to the mouse being pressed.
  /// @param _action is the action that the mouse performed (which button was clicked).
  ///-----------------------------------------------------------------------------------------------------
  virtual void handleMouseClick(const QMouseEvent& _action) = 0;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to rotate the camera based on mouse movement.
  /// @param _mousePos is the current mouse position.
  ///-----------------------------------------------------------------------------------------------------
  virtual void mouseRotate(const glm::vec2 &_mousePos) = 0;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to zoom the camera based on mouse movement.
  /// @param _mousePos is the current mouse position.
  ///-----------------------------------------------------------------------------------------------------
  virtual void mouseZoom(const glm::vec2 &_mousePos) = 0;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to access the view matrix of the camera.
  /// @return An immutable reference to the view matrix.
  ///-----------------------------------------------------------------------------------------------------
  const glm::mat4 &viewMatrix();
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to access the projection matrix of the camera.
  /// @return An immutable reference to the projection matrix.
  ///-----------------------------------------------------------------------------------------------------
  const glm::mat4 &projMatrix();
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to get the current cameras position.
  /// @return The eye position.
  ///-----------------------------------------------------------------------------------------------------
  virtual glm::vec3 getCameraEye() const noexcept = 0;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to access the original starting position of the camera.
  /// @return The cameras origin or start position.
  ///-----------------------------------------------------------------------------------------------------
  glm::vec3 getCameraOrigin() const noexcept;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the cameras target (what it is looking at).
  /// @param _x is the x coordinate of the new target.
  /// @param _y is the y coordinate of the new target.
  /// @param _z is the z coordinate of the new target.
  ///-----------------------------------------------------------------------------------------------------
  void setTarget(const float _x, const float _y, const float _z) noexcept;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the cameras origin (it's start position).
  /// @param _x is the x coordinate of the new origin.
  /// @param _y is the y coordinate of the new origin.
  /// @param _z is the z coordinate of the new origin.
  ///-----------------------------------------------------------------------------------------------------
  void setOrigin(const float _x, const float _y, float _z) noexcept;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to set last stored mouse position of the camera.
  /// @param _x is the x coordinate of the mouse position.
  /// @param _y is the y coordinate of the mouse position.
  ///-----------------------------------------------------------------------------------------------------
  void setMousePos(const float _mouseX, const float _mouseY);
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Used to set current camera field of view.
  /// @param _fov is the new field of view.
  ///-----------------------------------------------------------------------------------------------------
  void setFov(const float _fov);

protected:
  ///-----------------------------------------------------------------------------------------------------
  /// @brief View matrix.
  ///-----------------------------------------------------------------------------------------------------
  glm::mat4 m_viewMatrix = glm::mat4(1.0f);
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Projection matrix.
  ///-----------------------------------------------------------------------------------------------------
  glm::mat4 m_projectMatrix;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief Last recorded mouse position.
  ///-----------------------------------------------------------------------------------------------------
  glm::vec2 m_lastPos   = {0.0f, 0.0f};
  ///-----------------------------------------------------------------------------------------------------
  /// @brief The cameras target position.
  ///-----------------------------------------------------------------------------------------------------
  glm::vec3 m_target    = {0.0f, 0.0f, 0.0f};
  ///-----------------------------------------------------------------------------------------------------
  /// @brief The cameras start position.
  ///-----------------------------------------------------------------------------------------------------
  glm::vec3 m_camOrigin = {0.0f, 0.0f, 1.0f};
  ///-----------------------------------------------------------------------------------------------------
  /// @brief The cameras field of view (y axis).
  ///-----------------------------------------------------------------------------------------------------
  float m_fovy           = glm::quarter_pi<float>();
  ///-----------------------------------------------------------------------------------------------------
  /// @brief The cameras aspect ratio.
  ///-----------------------------------------------------------------------------------------------------
  float m_aspectRatio    = 1.0f;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief The cameras near clipping plane, we won't draw anything closer than this.
  ///-----------------------------------------------------------------------------------------------------
  float m_nearClippingPlane = 0.1f;
  ///-----------------------------------------------------------------------------------------------------
  /// @brief The cameras far clipping plane, we won't draw anything further away than this.
  ///-----------------------------------------------------------------------------------------------------
  float m_farClippingPlane  = 100.0f;

};

#endif // CAMERA_H

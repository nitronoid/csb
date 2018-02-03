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

// For basic stream output
#include <iostream>

// Used only for sin() and cos() functions
#include <math.h>

// Needed for matrix operations
#include <glm.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>

// Used to maintain a timer
#include <chrono>

#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>
//#include <gtx/string_cast.hpp>

/**
 * @brief The Camera class
 */
class Camera
{
public:
  /// Default Constructor
  Camera() = default;

  /// Default copy constructor
  Camera(const Camera&) = default;

  /// Default copy assignment operator
  Camera& operator=(const Camera&) = default;

  /// Default move constructor
  Camera(Camera&&) = default;

  /// Default move assignment operator
  Camera& operator=(Camera&&) = default;

  ///Virtual dtor
  virtual ~Camera();

  /// Trigger this when the window resizes
  virtual void resize(const int _width, const int _height);

  /// Call this before you need to retrieve the matrices from the camera
  virtual void update();

  /// Handle keypress / release events
  virtual void handleKey(const int _glfwKey, const bool _isPress) = 0;

  /// Mouse movement handler to look around
  virtual void handleMouseMove(const float _mouseX, const float _mouseY) = 0;

  /// Mouse click handler
  virtual void handleMouseClick(const QMouseEvent& _action) = 0;

  virtual void mouseRotate(float _mouseX, float _mouseY) = 0;
  virtual void mouseZoom(float, float _mouseY) = 0;

  /// Functions to retrieve matrices from the camera
  const glm::mat4 &viewMatrix();
  const glm::mat4 &projMatrix();

  /// Set the initial mouse position manually
  void setMousePos(float mouseX, float mouseY);

protected:
  /// Keep track of the last time
  using hr_clock =  std::chrono::high_resolution_clock;
  hr_clock::time_point m_lastTime =  hr_clock::now();

  /// Our view and project matrices are stored with the camera and retrieved as needed
  glm::mat4 m_viewMatrix;
  glm::mat4 m_projectMatrix;

  /// Last mouse coordinates
  glm::vec2 m_lastPos{0.0f, 0.0f};
  float m_lastX = 0.0f;
  float m_lastY = 0.0f;

  /// Keep track of the camera dimensions
  int m_windowWidth = 1;
  int m_windowHeight = 1;

  /// Keep track of the camera parameters
  float m_fovy = glm::pi<float>() * 0.25f;   //< Field of view in y
  float m_aspect = 1.0f; //< Aspect ratio
  float m_zNear = 1.0f;  //< Near clipping plane
  float m_zFar = 8.0f;   //< Far clipping plane

private:
  /// Return the elapsed time since this was last called
  double elapsedTime();
  /// A silly function to convert to radians (there are better ways to do this)
  double toRads(const double degs) const;

};

#endif // CAMERA_H

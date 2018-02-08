#include "CameraStates.h"
#include "Camera.h"
#include <QMouseEvent>
#include <QKeyEvent>


//-----------------------------------------------------------------------------------------------------
void CameraState::resetPosition(Camera* const io_camera, const int _key, const bool _isPress)
{
  // If the F key has been pressed
  if (_isPress && _key == Qt::Key_F)
    io_camera->resetPosition();
}
//-----------------------------------------------------------------------------------------------------
void CameraZoom::handleMouseMove(Camera*const io_camera, const glm::vec2 &_mousePos)
{
  // If the camera is in a zooming state, we interpret mouse movement as a zoom
  io_camera->mouseZoom(_mousePos);
}
//-----------------------------------------------------------------------------------------------------
void CameraZoom::handleKey(Camera*const io_camera, const int _key, const bool _isPress)
{
  resetPosition(io_camera, _key, _isPress);
}
//-----------------------------------------------------------------------------------------------------
void CameraRotate::handleMouseMove(Camera*const io_camera, const glm::vec2 &_mousePos)
{
  // If the camera is in a rotating state, we interpret mouse movement as a rotation
  io_camera->mouseRotate(_mousePos);
}
//-----------------------------------------------------------------------------------------------------
void CameraRotate::handleKey(Camera*const io_camera, const int _key, const bool _isPress)
{
  resetPosition(io_camera, _key, _isPress);
}
//-----------------------------------------------------------------------------------------------------
void CameraPassive::handleMouseMove(Camera*const , const glm::vec2 & )
{
  // Passive camera's do nothing with mouse movement
}
//-----------------------------------------------------------------------------------------------------
void CameraPassive::handleKey(Camera*const io_camera, const int _key, const bool _isPress)
{
  resetPosition(io_camera, _key, _isPress);
}
//-----------------------------------------------------------------------------------------------------

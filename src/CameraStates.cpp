#include "CameraStates.h"
#include "Camera.h"
#include <QMouseEvent>
#include <QKeyEvent>

void CameraState::resetPosition(Camera* const _camera, const int _key, const bool _isPress)
{
  if (_isPress && _key == Qt::Key_F)
    _camera->resetPosition();
}

void CameraZoom::handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos)
{
  _camera->mouseZoom(_mousePos);
}

void CameraZoom::handleKey(Camera*const _camera, const int _key, const bool _isPress)
{
  resetPosition(_camera, _key, _isPress);
}

void CameraRotate::handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos)
{
  _camera->mouseRotate(_mousePos);
}

void CameraRotate::handleKey(Camera*const _camera, const int _key, const bool _isPress)
{
  resetPosition(_camera, _key, _isPress);
}

void CameraPassive::handleMouseMove(Camera*const , const glm::vec2 & )
{}

void CameraPassive::handleKey(Camera*const _camera, const int _key, const bool _isPress)
{
  resetPosition(_camera, _key, _isPress);
}

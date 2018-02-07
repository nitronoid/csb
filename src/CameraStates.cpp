#include "CameraStates.h"
#include "Camera.h"
#include <QMouseEvent>

void CameraZoom::handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos)
{
  _camera->mouseZoom(_mousePos);
}

void CameraZoom::handleKey(Camera*const , const int , const bool )
{}

void CameraRotate::handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos)
{
  _camera->mouseRotate(_mousePos);
}

void CameraRotate::handleKey(Camera*const , const int , const bool )
{}

void CameraPassive::handleMouseMove(Camera*const , const glm::vec2 & )
{}

void CameraPassive::handleKey(Camera*const, const int , const bool )
{}

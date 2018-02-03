#include "CameraStates.h"
#include "Camera.h"
#include <QMouseEvent>

void CameraZoom::handleMouseMove(Camera*const _camera, const float _mouseX, const float _mouseY)
{
  _camera->mouseZoom(_mouseX, _mouseY);
}

void CameraZoom::handleKey(Camera*const , const int , const bool )
{}

void CameraRotate::handleMouseMove(Camera*const _camera, const float _mouseX, const float _mouseY)
{
  _camera->mouseRotate(_mouseX, _mouseY);
}

void CameraRotate::handleKey(Camera*const , const int , const bool )
{}

void CameraPassive::handleMouseMove(Camera*const , const float , const float )
{}

void CameraPassive::handleKey(Camera*const, const int , const bool )
{}

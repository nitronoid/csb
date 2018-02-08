#include "Camera.h"


//-----------------------------------------------------------------------------------------------------
void Camera::resize(const int _width, const int _height)
{
  // Check we won't get a divide by zero
  m_aspectRatio = (!_height)? 1.0f : (float(_width)/float(_height));
}
//-----------------------------------------------------------------------------------------------------
glm::vec3 Camera::getCameraOrigin() const noexcept
{
  return m_camOrigin;
}
//-----------------------------------------------------------------------------------------------------
void Camera::setTarget(const float _x, const float _y, const float _z) noexcept
{
  m_target.x = _x;
  m_target.y = _y;
  m_target.z = _z;
}
//-----------------------------------------------------------------------------------------------------
void Camera::setOrigin(const float _x, const float _y, float _z) noexcept
{
  m_camOrigin.x = _x;
  m_camOrigin.y = _y;
  m_camOrigin.z = _z;
}
//-----------------------------------------------------------------------------------------------------
void Camera::update()
{
  m_projectMatrix = glm::perspective( m_fovy, m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
}
//-----------------------------------------------------------------------------------------------------
void Camera::setMousePos(const float _mouseX, const float _mouseY)
{
  m_lastPos.x = _mouseX;
  m_lastPos.y = _mouseY;
}
//-----------------------------------------------------------------------------------------------------
void Camera::setFov(const float _fov)
{
  m_fovy = _fov;
}
//-----------------------------------------------------------------------------------------------------
void Camera::resetPosition()
{
  m_lastPos = {0.0f, 0.0f};
  m_target = {0.0f, 0.0f, -2.0f};
  m_camOrigin = {0.0f, 0.0f, 0.0f};
}
//-----------------------------------------------------------------------------------------------------
const glm::mat4& Camera::viewMatrix()
{
  return m_viewMatrix;
}
//-----------------------------------------------------------------------------------------------------
const glm::mat4& Camera::projMatrix()
{
  return m_projectMatrix;
}
//-----------------------------------------------------------------------------------------------------


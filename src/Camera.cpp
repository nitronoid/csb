#include "Camera.h"

/// This gives the compiler a place to store the virtual table
Camera::~Camera() = default;

/**
 * @brief Camera::resize
 * @param width
 * @param height
 */
void Camera::resize(const int _width, const int _height)
{
  m_windowWidth = _width;
  m_windowHeight = _height;
  m_aspectRatio = (_height == 0)? 1.0f : (float(_width)/float(_height));
}

/**
 * @brief Camera::elapsedTime
 * @return
 */
float Camera::elapsedTime()
{
  // save some typing
  using namespace std::chrono;
  auto now = hr_clock::now();
  float ret_val = duration_cast<milliseconds>(now - m_lastTime).count()  * 0.001f;
  m_lastTime = now;
  return ret_val;
}

float Camera::toRads(const float degs) const
{
  static constexpr float radianRatio = 3.141592654f / 180.0f;
  return degs * radianRatio;
}


/**
 * @brief Camera::update
 *  You should do something in this function to handle the calculation of the
 *  view and projection matrices V and P respectively
 */
void Camera::update()
{
  m_viewMatrix = glm::mat4(1.0f);
  m_projectMatrix = glm::perspective( m_fovy, m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
}


void Camera::setMousePos( float mouseX, float mouseY )
{
  m_lastPos.x = mouseX;
  m_lastPos.y = mouseY;
}

const glm::mat4& Camera::viewMatrix()
{
  return m_viewMatrix;
}

const glm::mat4& Camera::projMatrix()
{
  return m_projectMatrix;
}

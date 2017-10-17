#include "Camera.h"

/**
 *
 * This class was taken from a Rendering workshop
 * it belongs to Richard Southern
 *
 **/

Camera::Camera() :
    m_fovy(glm::pi<float>() * 0.25f),
    m_zNear(1.0f),
m_zFar(8.0f),
    m_lastX(0.0),
    m_lastY(0.0)
{
    resize(1, 1);
}

/**
 * @brief Camera::resize
 * @param width
 * @param height
 */
void Camera::resize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
		m_aspect = (height == 0)?1.0f:( float( width ) / float( height ) );
}


/**
 * @brief Camera::elapsedTime
 * @return
 */
double Camera::elapsedTime()
{
    auto now = std::chrono::high_resolution_clock::now();
    double ret_val = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastTime).count()  * 0.001;
    m_lastTime = now;
    return ret_val;
}


/**
 * @brief Camera::update
 *  You should do something in this function to handle the calculation of the
 *  view and projection matrices V and P respectively
 */
void Camera::update()
{
    m_V = glm::mat4(1.0f);
		m_P = glm::perspective( m_fovy, m_aspect, m_zNear, m_zFar );
}

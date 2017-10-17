#include "TrackballCamera.h"
#include <gtx/norm.hpp>
#include <gtc/constants.hpp>

/**
 * This class was taken from a Rendering workshop
 * it belongs to Richard Southern
 *
 * @brief TrackballCamera::TrackballCamera
 * Construct our trackball camera with some default parameters
 */
TrackballCamera::TrackballCamera() :
    Camera(),
    m_state(TRACKBALL_PASSIVE),
    m_yaw(0.0),
    m_pitch(0.0),
    m_zoom(1.0),
    m_sensitivity(0.01),
    m_lastYaw(0.0),
    m_lastPitch(0.0),
    m_dirty(true)
{
}

/**
 * @brief TrackballCamera::handleMouseClick
 * @param mouseX current mouse X coordinate
 * @param mouseY current mouse Y coordinate
 * @param button GLFW code for which mouse was clicked
 * @param mods GLFW code for key modifiers (e.g. CTRL etc)
 * This is effectively a state machine which determines what we're currently doing. Changes to the
 * mouse behaviour would need to be done here.
 */
void TrackballCamera::handleMouseClick(double mouseX, double mouseY, int button, QMouseEvent * action, int mods)
{
//	std::cout << "handling the clicks \n";
	switch(m_state)
	{
		case TRACKBALL_PASSIVE:
			if (action->type() == QMouseEvent::MouseButtonPress)
			{
				m_lastX = mouseX; m_lastY = mouseY;
				if (action->buttons() == Qt::LeftButton)
				{
//					std::cout << "rotating \n";
					m_state = TRACKBALL_ROTATING;
				}
				else if (action->buttons() == Qt::RightButton)
				{
//					std::cout << "Zooming \n";
					m_state = TRACKBALL_ZOOMING;
				}
			}
			break;
    case TRACKBALL_ROTATING:
			if (action->type() == QMouseEvent::MouseButtonRelease)
			{
				m_state = TRACKBALL_PASSIVE;
				m_lastYaw = m_yaw; m_lastPitch = m_pitch;
			}
			break;
    case TRACKBALL_ZOOMING:
			if (action->type() == QMouseEvent::MouseButtonRelease)
			{
            m_state = TRACKBALL_PASSIVE;
			}
			break;
    }
}

/**
 * @brief TrackballCamera::handleMouseMove
 * @param mouseX
 * @param mouseY
 * Pass the mouse move on to the relevent handler
 */
void TrackballCamera::handleMouseMove(double mouseX, double mouseY)
{
    switch (m_state)
      {
    case TRACKBALL_ROTATING:
        mouseRotate(mouseX, mouseY);
        break;
    case TRACKBALL_ZOOMING:
        mouseZoom(mouseX, mouseY);
        break;
    default:
        break;
    }
    m_dirty = true;
}

/**
 * @brief TrackballCamera::handleMouseMove
 * @param mouseX current mouse coordinate X
 * @param mouseY current mouse coordinate Y
 * This is the easiest trackball available. The mouse x difference maps to yaw angle and the y maps to pitch angle.
 */
void TrackballCamera::mouseRotate(double mouseX, double mouseY)
{
    // Make sure the yaw is reset when we go past to -pi,pi
    m_yaw = m_lastYaw + (m_lastX - mouseX)*m_sensitivity;
		if (m_yaw > glm::pi<double>())
			{
				m_yaw -= 2.0 * glm::pi<double>();
			}
		else if (m_yaw < -glm::pi<double>())
			{
				m_yaw += 2.0 * glm::pi<double>();
    }

    // Make sure our pitch is clamped within the range -pi/2,pi/2
    m_pitch = glm::clamp(m_lastPitch + (m_lastY - mouseY)*m_sensitivity, // value
                         -glm::pi<double>() * 0.5, // lower bound
                         glm::pi<double>() * 0.5); // upper bound
}

/**
 * @brief TrackballCamera::mouseZoom
 * @param mouseX
 * @param mouseY
 */
void TrackballCamera::mouseZoom(double mouseX, double mouseY)
{
	m_zoom = glm::clamp(m_zoom + (mouseY - m_lastY) * 0.01 * m_sensitivity, 0.0, 10.0);
}

/**
 * @brief TrackballCamera::update
 * The yaw and pitch are the rotations about the y and x axes respectively, constructed using angle and axis.
 * Note that in glm the matrix is constructed using quaternions - I could leave them in this form for the rotation.
 * In this case, the eye is rotated around the target
 */
void TrackballCamera::update()
{
	if (m_dirty)
		{
    // Call base class to update perspective
    Camera::update();

    // Now use lookat function to set the view matrix (assume y is up)
    glm::dmat3 R_yaw = glm::mat3_cast(glm::angleAxis(m_yaw, glm::dvec3(0.0, 1.0, 0.0)));
    glm::dmat3 R_pitch = glm::mat3_cast(glm::angleAxis(m_pitch, glm::dvec3(1.0, 0.0, 0.0)));
		glm::dvec3 eye = (R_yaw * R_pitch * (m_zoom * (m_eye - m_target))) + m_target;
		m_V = glm::lookAt(glm::vec3(eye), glm::vec3(m_target), glm::vec3(0.0f,1.0f,0.0f));

		m_dirty = false;
		}
}

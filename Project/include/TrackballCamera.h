#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H

///**********************************PLEASE NOTICE*****************************************///
///****************************************************************************************///
///*																																										  *///
///*	THIS CLASS BELONGS TO RICHARD SOUTHERN,                                             *///
///*	MINOR CHANGES WERE MADE TO MAKE SURE THE CAMERA WORKED WITH SDL											*///
///*  BUT IT SHOUDLN'T BE MARKED																												  *///
///*	THE ORIGINAL FILE IS FROM A RENDERING WORKSHOP (TRACKBALL - 2/2/2017)               *///
///*																																										  *///
///****************************************************************************************///
///****************************************************************************************///

#include "Camera.h"
#include <gtc/quaternion.hpp>
#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>

/**
 * @brief The TrackballCamera class
 */
class TrackballCamera : public Camera
{
    typedef enum
{
        TRACKBALL_PASSIVE,
        TRACKBALL_ROTATING,
        TRACKBALL_ZOOMING
    } TrackballState;
public:
    /// Construct our empty trackball (no destructor needed as we don't allocate dynamic memory)
    TrackballCamera();

    /// Call this before you need to retrieve the matrices from the camera
    virtual void update();

    /// Mouse movement handler to look around
    virtual void handleMouseMove(double /*mouseX*/, double /*mouseY*/);

    /// Mouse click handler
    virtual void handleMouseClick(double /*mouseX*/, double /*mouseY*/, int /*button*/, QMouseEvent * /*action*/, int /*mods*/);

    /// Set the direction you're looking
    void setTarget(const double& x, const double& y, const double& z) {m_target = glm::dvec3(x,y,z);}

    /// Set the position that our camera is
		void setEye(const double& x, const double& y, const double& z)
		{
			m_eye = glm::dvec3(x,y,z);

//			m_V = glm::lookAt(glm::vec3(m_eye), glm::vec3(m_target), glm::vec3(0.0f,1.0f,0.0f));

		}

    /// Set the zoom by scaling the eye position
    void setZoom(const double& zoom = 1.0) noexcept {m_zoom = zoom;}

    /// Controller sensitivity
    void setSensitivity(const double& sensitivity=0.01) noexcept {m_sensitivity = sensitivity;}

		glm::vec3 getEye() const { return m_eye; }
		glm::mat4 getM_V() const { return m_V; }
//		void setM_V( glm::mat4 _m_v ) { m_V = _m_v; }
private:
    /// Rotate based on the current mouse position and the mouse click position
    void mouseRotate(double /*xpos*/, double /*ypos*/);

    /// Zoom based on the current mouse position and the position of the mouse click
    void mouseZoom(double /*xpos*/, double /*ypos*/);

    /// Keep track of our current trackball state
    TrackballState m_state;

    /// Store the yaw and pitch
    double m_yaw, m_pitch, m_zoom, m_sensitivity;

    /// Store the target and position with this class
    glm::dvec3 m_target, m_eye;

    /// Store the last yaw and pitch so that the trackball stays put when the mouse is released
    double m_lastYaw, m_lastPitch;

    /// Keep track of whether the matrices need to be rebuilt
    bool m_dirty;
};

#endif // TRACKBALLCAMERA_H

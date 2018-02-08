#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H

/// Modified from :-
/// Richard Southern (02/02/2017). Trackball workshop.
/// @note Changes were made to the interface of camera, mouse rotate and zoom are pure virtual.
/// @note The case statements have been removed in favour of CameraStates which use double dispatch,
/// based on the camera and cameras state, to act accordingly.
/// @note The bug with zooming was fixed by updating the last mouse position.
/// @note Rotation locking bug was fixed using a small epsilon before clamping.

#include "Camera.h"
#include <memory>
#include "CameraStates.h"

/**
 * @brief The TrackballCamera class
 */
class TrackballCamera : public Camera
{

public:
  /// Construct our empty trackball (no destructor needed as we don't allocate dynamic memory)
  TrackballCamera() = default;

  /// Default virtual destructor
  ~TrackballCamera() override = default;

  /// Call this before you need to retrieve the matrices from the camera
  virtual void update() override;

  /// Mouse movement handler to look around
  virtual void handleMouseMove(const glm::vec2 &_mousePos) override;

  /// Handle keypress / release events
  virtual void handleKey(const int _key, const bool _isPress) override;

  /// Mouse click handler
  virtual void handleMouseClick(const QMouseEvent &_action) override;

  /// Set the zoom by scaling the eye position
  void setZoom(const float _zoom) noexcept;

  /// Controller sensitivity
  void setSensitivity(const float sensitivity) noexcept;

  /// Rotate based on the current mouse position and the mouse click position
  virtual void mouseRotate(const glm::vec2 &_mousePos) override;

  /// Zoom based on the current mouse position and the position of the mouse click
  virtual void mouseZoom(const glm::vec2 &_mousePos) override;

  /// Get the eye vector
  virtual glm::vec3 getCameraEye() const noexcept override;

  virtual void resetPosition() override;

private:
  void updateYawPitch();

private:
  enum CAM_STATE
  {
    TRACKBALL_ZOOMING  = 0,
    TRACKBALL_ROTATING = 1,
    TRACKBALL_PASSIVE  = 2
  };

  using statePtr = std::unique_ptr<CameraState>;
  static const statePtr m_states[];

  /// Keep track of our current trackball state
  CAM_STATE m_currentState = TRACKBALL_PASSIVE;

  /// Store the yaw and pitch
  float m_yaw         = 0.0f;
  float m_pitch       = 0.0f;
  float m_zoom        = 2.5f;

  /// Store the last yaw and pitch so that the trackball stays put when the mouse is released
  float m_lastYaw     = 0.0f;
  float m_lastPitch   = 0.0f;


  float m_sensitivity = 0.01f;

};

#endif // TRACKBALLCAMERA_H

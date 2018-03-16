#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H

//-------------------------------------------------------------------------------------------------------
/// @author Jack Diver
/// Modified from :-
/// Richard Southern (02/02/2017). Trackball workshop.
/// @note Changes were made to the interface of camera, mouse rotate and zoom are pure virtual.
/// @note The case statements have been removed in favour of CameraStates which use double dispatch,
/// based on the camera and camera's state, to act accordingly.
/// @note The bug with zooming was fixed by updating the last mouse position.
/// @note Rotation locking bug was fixed using a small epsilon before clamping.
//-------------------------------------------------------------------------------------------------------

#include "Camera.h"
#include <memory>
#include "CameraStates.h"

/**
 * @brief The TrackballCamera class
 */
class TrackballCamera : public Camera
{
  //-----------------------------------------------------------------------------------------------------
  /// @brief Enum used to indicate the current state of the camera.
  //-----------------------------------------------------------------------------------------------------
  enum CAM_STATE
  {
    TRACKBALL_ZOOMING  = 0,
    TRACKBALL_ROTATING = 1,
    TRACKBALL_PASSIVE  = 2
  };
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  TrackballCamera() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  TrackballCamera(const TrackballCamera&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  TrackballCamera& operator=(const TrackballCamera&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  TrackballCamera(TrackballCamera&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  TrackballCamera& operator=(TrackballCamera&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  ~TrackballCamera() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Update function should be called to update the camera matrices, before retrieving them.
  //-----------------------------------------------------------------------------------------------------
  virtual void update() override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Handles key press events by calling the current camera state function.
  /// @param [in] _key is the Qt key that has been pressed/released.
  /// @param [in] _isPress whether this key has been pressed or released.
  //-----------------------------------------------------------------------------------------------------
  virtual void handleKey(const int _key, const bool _isPress) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to respond to mouse movement by calling the current camera state function.
  /// @param [in] _mousePos is the new position of the mouse.
  //-----------------------------------------------------------------------------------------------------
  virtual void handleMouseMove(const glm::vec2 &_mousePos) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to respond to the mouse being pressed by calling the current camera state function.
  /// @param [in] _action is the action that the mouse performed (which button was clicked).
  //-----------------------------------------------------------------------------------------------------
  virtual void handleMouseClick(const QMouseEvent &_action) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the current zoom of the camera.
  /// @param [in] _zoom is the new zoom value.
  //-----------------------------------------------------------------------------------------------------
  void setZoom(const float _zoom) noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the current sensitivity of the camera.
  /// @param [in] _sensitivity is the new sensitivity value.
  //-----------------------------------------------------------------------------------------------------
  void setSensitivity(const float _sensitivity) noexcept;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to rotate the camera based on mouse movement.
  /// @param [in] _mousePos is the current mouse position.
  //-----------------------------------------------------------------------------------------------------
  virtual void mouseRotate(const glm::vec2 &_mousePos) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to zoom the camera based on mouse movement.
  /// @param [in] _mousePos is the current mouse position.
  //-----------------------------------------------------------------------------------------------------
  virtual void mouseZoom(const glm::vec2 &_mousePos) override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to get the current cameras position.
  /// @return The eye position.
  //-----------------------------------------------------------------------------------------------------
  virtual glm::vec3 getCameraEye() const noexcept override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Resets the camera to it's default position and orientation.
  //-----------------------------------------------------------------------------------------------------
  virtual void resetPosition() override;

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the last yaw and pitch to the current values.
  //-----------------------------------------------------------------------------------------------------
  void updateYawPitch();

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Alias for camera state pointers.
  //-----------------------------------------------------------------------------------------------------
  using statePtr = std::unique_ptr<CameraState>;
  //-----------------------------------------------------------------------------------------------------
  /// @brief An array of states for this camera, which can be indexed by the current state. The state
  /// objects them-selves contain no data, so this variable can be static and const.
  //-----------------------------------------------------------------------------------------------------
  static const statePtr m_states[];
  //-----------------------------------------------------------------------------------------------------
  /// @brief The current state of the camera.
  //-----------------------------------------------------------------------------------------------------
  CAM_STATE m_currentState = TRACKBALL_PASSIVE;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The current sensitivity of the camera.
  //-----------------------------------------------------------------------------------------------------
  float m_sensitivity = 0.01f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The current zoom of the camera.
  //-----------------------------------------------------------------------------------------------------
  float m_zoom        = 2.5f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The current yaw of the camera.
  //-----------------------------------------------------------------------------------------------------
  float m_yaw         = 0.0f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The current pitch of the camera.
  //-----------------------------------------------------------------------------------------------------
  float m_pitch       = 0.0f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The last yaw of the camera.
  //-----------------------------------------------------------------------------------------------------
  float m_lastYaw     = 0.0f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The last pitch of the camera.
  //-----------------------------------------------------------------------------------------------------
  float m_lastPitch   = 0.0f;

};

#endif // TRACKBALLCAMERA_H

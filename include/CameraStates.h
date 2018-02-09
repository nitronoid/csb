#ifndef CAMERASTATES_H
#define CAMERASTATES_H

#include "vec2.hpp"

class Camera;

class CameraState
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  CameraState() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  CameraState(const CameraState&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  CameraState& operator=(const CameraState&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  CameraState(CameraState&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  CameraState& operator=(CameraState&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default virtual destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~CameraState() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to respond to mouse movement.
  /// @param [io] io_camera is the camera that holds this state.
  /// @param [in] _mousePos is the new position of the mouse.
  //-----------------------------------------------------------------------------------------------------
  virtual void handleMouseMove(Camera*const io_camera, const glm::vec2 &_mousePos) = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Handles key press events.
  /// @param [io] io_camera is the camera that holds this state.
  /// @param [in] _key is the Qt key that has been pressed/released.
  /// @param [in] _isPress whether this key has been pressed or released.
  //-----------------------------------------------------------------------------------------------------
  virtual void handleKey(Camera*const io_camera, const int _key, const bool _isPress) = 0;

protected:
  //-----------------------------------------------------------------------------------------------------
  /// @brief This is a function that resets the camera to it's default position based on a key press,
  /// it has been included as for my example, I wanted the same behaviour for all states.
  /// @param [io] io_camera is the camera that holds this state.
  /// @param [in] _key is the Qt key that has been pressed/released.
  /// @param [in] _isPress whether this key has been pressed or released.
  //-----------------------------------------------------------------------------------------------------
  void resetPosition(Camera* const io_camera, const int _key, const bool _isPress);
};

//-----------------------------------------------------------------------------------------------------
/// @brief Class that represents the behaviour of a zooming camera.
//-----------------------------------------------------------------------------------------------------
class CameraZoom : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const io_camera, const glm::vec2 &_mousePos) override;
  virtual void handleKey(Camera*const io_camera, const int _key, const bool _isPress) override;
};

//-----------------------------------------------------------------------------------------------------
/// @brief Class that represents the behaviour of a rotating camera.
//-----------------------------------------------------------------------------------------------------
class CameraRotate : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const io_camera, const glm::vec2 &_mousePos) override;
  virtual void handleKey(Camera*const io_camera, const int _key, const bool _isPress) override;
};

//-----------------------------------------------------------------------------------------------------
/// @brief Class that represents the behaviour of a passive camera.
//-----------------------------------------------------------------------------------------------------
class CameraPassive : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const io_camera, const glm::vec2 &_mousePos) override;
  virtual void handleKey(Camera * const io_camera, const int _key, const bool _isPress) override;
};


#endif // CAMERASTATES_H

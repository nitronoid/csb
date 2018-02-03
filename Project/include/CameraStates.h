#ifndef TRACKBALLCOMMANDS_H
#define TRACKBALLCOMMANDS_H

class Camera;

class CameraState
{
public:
  /// Ctor's dtor's op's
  CameraState() = default;
  virtual ~CameraState() = default;
  CameraState(const CameraState&) = default;
  CameraState& operator=(const CameraState&) = default;
  CameraState(CameraState&&) = default;
  CameraState& operator=(CameraState&&) = default;

  virtual void handleMouseMove(Camera*const _camera, const float _mouseX, const float _mouseY) = 0;
  virtual void handleKey(Camera*const _camera, const int _glfwKey, const bool _isPress) = 0;

};

class CameraZoom : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const _camera, const float _mouseX, const float _mouseY) override;
  virtual void handleKey(Camera*const _camera, const int _glfwKey, const bool _isPress) override;
};

class CameraRotate : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const _camera, const float _mouseX, const float _mouseY) override;
  virtual void handleKey(Camera*const _camera, const int _glfwKey, const bool _isPress) override;
};

class CameraPassive : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const, const float, const float) override;
  virtual void handleKey(Camera * const _camera, const int _glfwKey, const bool _isPress) override;
};


#endif // TRACKBALLCOMMANDS_H

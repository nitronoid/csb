#ifndef TRACKBALLCOMMANDS_H
#define TRACKBALLCOMMANDS_H

#include "vec2.hpp"

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

  virtual void handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos) = 0;
  virtual void handleKey(Camera*const _camera, const int _key, const bool _isPress) = 0;

protected:
  void resetPosition(Camera* const _camera, const int _key, const bool _isPress);
};

class CameraZoom : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos) override;
  virtual void handleKey(Camera*const _camera, const int _key, const bool _isPress) override;
};

class CameraRotate : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos) override;
  virtual void handleKey(Camera*const _camera, const int _key, const bool _isPress) override;
};

class CameraPassive : public CameraState
{
public:
  virtual void handleMouseMove(Camera*const _camera, const glm::vec2 &_mousePos) override;
  virtual void handleKey(Camera * const _camera, const int _key, const bool _isPress) override;
};


#endif // TRACKBALLCOMMANDS_H

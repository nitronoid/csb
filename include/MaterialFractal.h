#ifndef MATERIALFRACTAL_H
#define MATERIALFRACTAL_H

#include "Material.h"
#include <chrono>

class Camera;

class MaterialFractal : public Material
{
public:
  MaterialFractal(const std::shared_ptr<Camera> &io_camera, const std::shared_ptr<ShaderLib> &io_shaderLib, std::array<glm::mat4, 3>* io_matrices) :
    Material(io_camera, io_shaderLib, io_matrices)
  {}
  MaterialFractal(const MaterialFractal&) = default;
  MaterialFractal& operator=(const MaterialFractal&) = default;
  MaterialFractal(MaterialFractal&&) = default;
  MaterialFractal& operator=(MaterialFractal&&) = default;
  ~MaterialFractal() override = default;

  virtual void init() override;

  virtual void update() override;

  virtual const char* shaderFileName() const override;

  virtual void handleKey(QKeyEvent* io_event, QOpenGLContext* io_context) override;

private:
  using hrclock = std::chrono::high_resolution_clock;
  hrclock::time_point m_start = hrclock::now();
  float m_elapsed = 0.0f;
  bool m_updateTime = true;
};


#endif // MATERIALFRACTAL_H

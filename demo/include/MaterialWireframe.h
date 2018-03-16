#ifndef MATERIALWIREFRAME_H
#define MATERIALWIREFRAME_H

#include "Material.h"

class Camera;

class MaterialWireframe : public Material
{
public:
  MaterialWireframe(const std::shared_ptr<Camera> &io_camera, const std::shared_ptr<ShaderLib> &io_shaderLib, std::array<glm::mat4, 3>* io_matrices) :
    Material(io_camera, io_shaderLib, io_matrices)
  {}
  MaterialWireframe(const MaterialWireframe&) = default;
  MaterialWireframe& operator=(const MaterialWireframe&) = default;
  MaterialWireframe(MaterialWireframe&&) = default;
  MaterialWireframe& operator=(MaterialWireframe&&) = default;
  ~MaterialWireframe() override = default;

  virtual void init() override;

  virtual void update() override;

  virtual const char* shaderFileName() const override;

};

#endif // MATERIALWIREFRAME_H

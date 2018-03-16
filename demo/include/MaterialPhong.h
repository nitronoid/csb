#ifndef MATERIALPHONG_H
#define MATERIALPHONG_H

#include "Material.h"

class Camera;

class MaterialPhong : public Material
{
public:
  MaterialPhong(const std::shared_ptr<Camera> &io_camera, const std::shared_ptr<ShaderLib> &io_shaderLib, std::array<glm::mat4, 3>* io_matrices) :
    Material(io_camera, io_shaderLib, io_matrices)
  {}
  MaterialPhong(const MaterialPhong&) = default;
  MaterialPhong& operator=(const MaterialPhong&) = default;
  MaterialPhong(MaterialPhong&&) = default;
  MaterialPhong& operator=(MaterialPhong&&) = default;
  ~MaterialPhong() override = default;

  virtual void init() override;

  virtual void update() override;

  virtual const char* shaderFileName() const override;

};

#endif // MATERIALPHONG_H

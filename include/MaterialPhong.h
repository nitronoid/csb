#ifndef MATERIALPHONG_H
#define MATERIALPHONG_H

#include "Material.h"

class Camera;

class MaterialPhong : public Material
{
public:
  MaterialPhong(ShaderLib *io_shaderLib, std::array<glm::mat4, 3>* io_matrices, Camera* _cam) :
    Material(io_shaderLib, io_matrices),
    m_cam(_cam)
  {}
  MaterialPhong(const MaterialPhong&) = default;
  MaterialPhong& operator=(const MaterialPhong&) = default;
  MaterialPhong(MaterialPhong&&) = default;
  MaterialPhong& operator=(MaterialPhong&&) = default;
  ~MaterialPhong() override = default;

  virtual void init() override;

  virtual void update() override;

  virtual const char* shaderFileName() const override;


private:
  Camera* m_cam = nullptr;
};

#endif // MATERIALPHONG_H

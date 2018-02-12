#ifndef MATERIALPHONG_H
#define MATERIALPHONG_H

#include "Material.h"

class Camera;

class MaterialPhong : public Material
{
public:
  MaterialPhong(Camera* _cam) :
    Material(),
    m_cam(_cam)
  {}
  MaterialPhong(const MaterialPhong&) = default;
  MaterialPhong& operator=(const MaterialPhong&) = default;
  MaterialPhong(MaterialPhong&&) = default;
  MaterialPhong& operator=(MaterialPhong&&) = default;
  ~MaterialPhong() override = default;

  virtual void init(ShaderLib* io_shaderLib, const size_t _index, std::array<glm::mat4, 3>* io_matrices) override;

  virtual void update() override;

  virtual const char* vertexName() const override;

  virtual const char* fragName() const override;

private:
  Camera* m_cam = nullptr;
};

#endif // MATERIALPHONG_H

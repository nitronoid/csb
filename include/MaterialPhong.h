#ifndef MATERIALPHONG_H
#define MATERIALPHONG_H

#include "Material.h"

class MaterialPhong : public Material
{
public:
  MaterialPhong() = default;
  MaterialPhong(const MaterialPhong&) = default;
  MaterialPhong& operator=(const MaterialPhong&) = default;
  MaterialPhong(MaterialPhong&&) = default;
  MaterialPhong& operator=(MaterialPhong&&) = default;
  ~MaterialPhong() override = default;

  virtual void init(ShaderProgram* io_shader, std::array<glm::mat4, 3>* io_matrices) override;

  virtual void update() override;

  virtual const char* vertexName() const override;

  virtual const char* fragName() const override;
};

#endif // MATERIALPHONG_H

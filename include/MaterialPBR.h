#ifndef MATERIALPBR_H
#define MATERIALPBR_H

#include "Material.h"
#include "vec3.hpp"

class Camera;

class MaterialPBR : public Material
{
public:
  MaterialPBR(ShaderLib *io_shaderLib, std::array<glm::mat4, 3>* io_matrices, Camera* _cam, const glm::vec3 &_albedo) :
    Material(io_shaderLib, io_matrices),
    m_cam(_cam),
    m_albedo(_albedo)
  {}
  MaterialPBR(const MaterialPBR&) = default;
  MaterialPBR& operator=(const MaterialPBR&) = default;
  MaterialPBR(MaterialPBR&&) = default;
  MaterialPBR& operator=(MaterialPBR&&) = default;
  ~MaterialPBR() override = default;

  virtual void init() override;

  virtual void update() override;

  virtual const char* shaderFileName() const override;


private:
  Camera* m_cam = nullptr;
  glm::vec3 m_albedo;

};

#endif // MATERIALPBR_H

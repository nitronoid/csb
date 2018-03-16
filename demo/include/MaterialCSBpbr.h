#ifndef MATERIALCSBPBR_H
#define MATERIALCSBPBR_H

#include "Material.h"

class MaterialCSBpbr : public Material
{
public:
  MaterialCSBpbr(
      const std::shared_ptr<Camera> &io_camera,
      const std::shared_ptr<ShaderLib> &io_shaderLib,
      std::array<glm::mat4, 3>* io_matrices,
      const glm::vec3 &_albedo,
      const float _ao,
      const float _exposure,
      const float _roughness,
      const float _metallic
      ) :
    Material(io_camera, io_shaderLib, io_matrices),
    m_albedo(_albedo),
    m_ao(_ao),
    m_exposure(_exposure),
    m_roughness(_roughness),
    m_metallic(_metallic)
  {}
  MaterialCSBpbr(const MaterialCSBpbr&) = default;
  MaterialCSBpbr& operator=(const MaterialCSBpbr&) = default;
  MaterialCSBpbr(MaterialCSBpbr&&) = default;
  MaterialCSBpbr& operator=(MaterialCSBpbr&&) = default;
  ~MaterialCSBpbr() override = default;

  virtual void init() override;

  virtual void update() override;

  virtual const char* shaderFileName() const override;


private:
  glm::vec3 m_albedo;
  float m_ao;
  float m_exposure;
  float m_roughness;
  float m_metallic;

};

#endif // MATERIALCSBPBR_H

#ifndef MATERIALPBR_H
#define MATERIALPBR_H

#include "Material.h"

class Camera;

class MaterialPBR : public Material
{
public:
  MaterialPBR(Camera* _cam) :
    Material(),
    m_cam(_cam)
  {}
  MaterialPBR(const MaterialPBR&) = default;
  MaterialPBR& operator=(const MaterialPBR&) = default;
  MaterialPBR(MaterialPBR&&) = default;
  MaterialPBR& operator=(MaterialPBR&&) = default;
  ~MaterialPBR() override = default;

  virtual void init(ShaderLib* io_shaderLib, const size_t _index, std::array<glm::mat4, 3>* io_matrices) override;

  virtual void update() override;

  virtual const char* vertexName() const override;

  virtual const char* fragName() const override;

private:
  Camera* m_cam = nullptr;

};

#endif // MATERIALPBR_H

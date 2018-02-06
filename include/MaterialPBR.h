#ifndef MATERIALPBR_H
#define MATERIALPBR_H

#include "Material.h"

class Camera;

class MaterialPBR : public Material
{
public:
  MaterialPBR() = default;
  MaterialPBR(Camera* _cam) :
    Material(),
    m_cam(_cam)
  {}
  MaterialPBR(const MaterialPBR&) = default;
  MaterialPBR& operator=(const MaterialPBR&) = default;
  MaterialPBR(MaterialPBR&&) = default;
  MaterialPBR& operator=(MaterialPBR&&) = default;
  ~MaterialPBR() override = default;

  virtual void init(ShaderProgram* _shader) override;

  virtual void update() override;

private:
  Camera* m_cam = nullptr;

};

#endif // MATERIALPBR_H

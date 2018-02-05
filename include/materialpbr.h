#ifndef MATERIALPBR_H
#define MATERIALPBR_H

#include "material.h"

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
  ~MaterialPBR() = default;

  virtual void setup(ShaderProgram* _shader) override;

  virtual void update(ShaderProgram* _shader) override;

private:
  Camera* m_cam = nullptr;

};

#endif // MATERIALPBR_H

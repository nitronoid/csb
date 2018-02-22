#ifndef MATERIALENVMAP_H
#define MATERIALENVMAP_H

#include "Material.h"
#include <QOpenGLTexture>
#include <QImage>

class Camera;

class MaterialEnvMap : public Material
{
public:
  MaterialEnvMap(const std::shared_ptr<Camera> &io_camera, const std::shared_ptr<ShaderLib> &io_shaderLib, std::array<glm::mat4, 3>* io_matrices) :
    Material(io_camera, io_shaderLib, io_matrices)
  {}
  MaterialEnvMap(const MaterialEnvMap&) = default;
  MaterialEnvMap& operator=(const MaterialEnvMap&) = default;
  MaterialEnvMap(MaterialEnvMap&&) = default;
  MaterialEnvMap& operator=(MaterialEnvMap&&) = default;
  ~MaterialEnvMap() override = default;

  virtual void init() override;

  virtual void update() override;

  virtual const char* shaderFileName() const override;

private:
  void initEnvMap();
  void initGlossMap();
  std::unique_ptr<QOpenGLTexture> m_envMap;
  std::unique_ptr<QOpenGLTexture> m_glossMap;

};

#endif // MATERIALENVMAP_H

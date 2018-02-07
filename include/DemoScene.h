#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "Scene.h"
#include "MaterialPBR.h"


class DemoScene : public Scene
{
  Q_OBJECT
public:
  DemoScene(Camera* io_camera, QWidget *_parent) :
    Scene(io_camera, _parent),
    m_material(new MaterialPBR(io_camera))
  {}

  DemoScene(const DemoScene&) = default;
  DemoScene& operator=(const DemoScene&) = default;
  DemoScene(DemoScene&&) = default;
  DemoScene& operator=(DemoScene&&) = default;
  /// @brief dtor
  ~DemoScene() override = default;

  virtual void init() override;

public slots:
  void rotating( const bool _rotating ) { m_rotating = _rotating; }
  void generateNewGeometry();

private:
  void loadMesh();
  virtual void renderScene() override;

private:
  size_t m_meshIndex = 0;
  //----------------------------------------------------------------------------------------------------------------------
  Mesh* m_mesh;
  //----------------------------------------------------------------------------------------------------------------------
  std::array<Mesh, 5> m_meshes;
  //----------------------------------------------------------------------------------------------------------------------
  ShaderProgram m_shaderProgram;
  //----------------------------------------------------------------------------------------------------------------------
  std::unique_ptr<Material> m_material;
  //----------------------------------------------------------------------------------------------------------------------
  bool m_rotating;
  //----------------------------------------------------------------------------------------------------------------------
  Buffer m_buffer;


};

#endif // DEMOSCENE_H

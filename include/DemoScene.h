#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "Scene.h"
#include "MaterialPBR.h"
#include "MaterialPhong.h"
#include "ShaderLib.h"


class DemoScene : public Scene
{
  Q_OBJECT
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor for DemoScene.
  /// @param [io] io_camera the camera used to view the scene.
  /// @param [io] io_shaderLib the shader library to store and retrieve our shaders.
  /// @param [io] io_parent the parent window to create the GL context in.
  //-----------------------------------------------------------------------------------------------------
  DemoScene(
      const std::shared_ptr<Camera> &io_camera,
      const std::shared_ptr<ShaderLib> &io_shaderLib,
      QWidget *_parent
      ) :
    Scene(io_camera, _parent),
    m_shaderLib(io_shaderLib)
  {}
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  DemoScene(const DemoScene&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  DemoScene& operator=(const DemoScene&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  DemoScene(DemoScene&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  DemoScene& operator=(DemoScene&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  ~DemoScene() override = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to intialise the scene, must call the base class init.
  //-----------------------------------------------------------------------------------------------------
  virtual void init() override;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to intialise the models, vbo and vao.
  //-----------------------------------------------------------------------------------------------------
  void initGeo();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to create our shader programs, or use exisiting ones if they have been loaded.
  //-----------------------------------------------------------------------------------------------------
  void initMaterials();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Receives and acts on a key event.
  /// @param [io] io_event is the key event that was received.
  //-----------------------------------------------------------------------------------------------------
  virtual void keyPress(QKeyEvent* io_event) override;

public slots:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to link a Qt button to the scene, to allow rotation of the model to be toggled.
  /// @param [in] _rotating tells the scene whether it should rotate the model or not.
  //-----------------------------------------------------------------------------------------------------
  void rotating(const bool _rotating);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to link a Qt button to the scene, to allow switching between meshes in the scene, this
  /// calls loadMesh.
  //-----------------------------------------------------------------------------------------------------
  void generateNewGeometry();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to link a Qt button to the scene, to cycle through the materials and apply them to
  /// the current Mesh.
  //-----------------------------------------------------------------------------------------------------
  void nextMaterial();

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to write our mesh data into the vbo.
  //-----------------------------------------------------------------------------------------------------
  void writeMeshAttributes();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to pass attribute pointers to the current shader program.
  //-----------------------------------------------------------------------------------------------------
  void setAttributeBuffers();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Must call the base class function, it then applies our shader and draws the current mesh.
  //-----------------------------------------------------------------------------------------------------
  virtual void renderScene() override;

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Holds our test meshes.
  //-----------------------------------------------------------------------------------------------------
  std::array<Mesh, 6> m_meshes;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Wraps up our OpenGL buffers and VAO.
  //-----------------------------------------------------------------------------------------------------
  MeshVBO m_meshVBO;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Vertex array object, default constructed with a pointer to this OpenGL widget,
  /// a dynamic_cast is used due to Scene's multiple inheritence.
  //-----------------------------------------------------------------------------------------------------
  std::unique_ptr<QOpenGLVertexArrayObject> m_vao {
    new QOpenGLVertexArrayObject(dynamic_cast<QObject*>(this))
  };
  //-----------------------------------------------------------------------------------------------------
  /// @brief A pointer to the shader library used by this scene.
  //-----------------------------------------------------------------------------------------------------
  std::shared_ptr<ShaderLib> m_shaderLib;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The materials used in this scene.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::unique_ptr<Material>> m_materials;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Holds the index of the currently drawn mesh in our array of meshes.
  //-----------------------------------------------------------------------------------------------------
  size_t m_meshIndex = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The current material.
  //-----------------------------------------------------------------------------------------------------
  size_t m_currentMaterial = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Is the mesh rotating.
  //-----------------------------------------------------------------------------------------------------
  bool m_rotating = false;


};

#endif // DEMOSCENE_H

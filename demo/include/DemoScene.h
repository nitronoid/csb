#ifndef CSBSCENE_H
#define CSBSCENE_H

#include "Scene.h"
#include "Material.h"
#include "ShaderLib.h"
#include "SimulatedMesh.h"
#include "Solver.h"
#include <QOpenGLFunctions_4_1_Core>

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
  /// @brief Used to link a Qt button to the scene, to allow pausing the timer.
  /// @param [in] _isPaused tells the solver whether or not it should step.
  //-----------------------------------------------------------------------------------------------------
  void paused(const bool _isPaused);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to link a Qt button to the scene, to cycle through the materials and apply them to
  /// the current Mesh.
  //-----------------------------------------------------------------------------------------------------
  void nextMaterial();

private:

  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to intialise the models, vbo and vao.
  //-----------------------------------------------------------------------------------------------------
  void initDemo1();
  void initDemo2();
  void initDemo3();
  void initDemo4();
  void prepMeshesGL();
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
  csb::Solver m_solver;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Holds our test meshes.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::shared_ptr<csb::SimulatedMesh>> m_meshes;
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
  /// @brief The current material.
  //-----------------------------------------------------------------------------------------------------
  size_t m_currentMaterial = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Is the mesh rotating.
  //-----------------------------------------------------------------------------------------------------
  bool m_rotating = false;

  std::vector<std::array<int, 3>> m_meshAttributeOffsets;
  std::vector<int> m_meshIndexStartPoints;
  std::vector<GLsizei> m_numIndicesPerMesh;
  std::vector<GLvoid*> m_meshIndexOffsets;
  std::vector<GLint> m_meshBaseVert;


  QOpenGLFunctions_4_1_Core *m_qogl_funcs;

};

#endif // CSBSCENE_H

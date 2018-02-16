#ifndef MATERIAL_H
#define MATERIAL_H

#include <array>
#include <mat4x4.hpp>
#include <memory>
#include <QKeyEvent>
#include "ShaderLib.h"
#include "Camera.h"

class Material
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Constructor
  /// @param [io] io_camera is the camera that is viewing the material
  /// @param [io] io_shaderLib contains the shader program that this material affects.
  /// @param [io] io_matrices is a pointer to the scene matrices used for the vertex shader.
  //-----------------------------------------------------------------------------------------------------
  Material(
      const std::shared_ptr<Camera> &io_camera,
      const std::shared_ptr<ShaderLib> &io_shaderLib,
      std::array<glm::mat4, 3>* io_matrices
      ) :
    m_shaderLib(io_shaderLib),
    m_matrices(io_matrices),
    m_cam(io_camera)
  {}
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  Material(const Material&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Material& operator=(const Material&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  Material(Material&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Material& operator=(Material&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default virtual destructor.
  //-----------------------------------------------------------------------------------------------------
  virtual ~Material();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to intialise a passed shader, subclasses must call this base function.
  //-----------------------------------------------------------------------------------------------------
  virtual void init() = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set the name of the shader that this material should be applied to.
  //-----------------------------------------------------------------------------------------------------
  void setShaderName(const std::string &_name);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to update shader values.
  //-----------------------------------------------------------------------------------------------------
  virtual void update() = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to set this as the active shader, and pass the uniform values stored in this material.
  //-----------------------------------------------------------------------------------------------------
  void apply();
  //-----------------------------------------------------------------------------------------------------
  /// @brief The file name of the json shader file that this material works with.
  //-----------------------------------------------------------------------------------------------------
  virtual const char* shaderFileName() const = 0;

  virtual void handleKey(QKeyEvent* io_event, QOpenGLContext* io_context);

protected:
  //-----------------------------------------------------------------------------------------------------
  /// @brief A pointer to the central Shader Library.
  //-----------------------------------------------------------------------------------------------------
  std::shared_ptr<ShaderLib> m_shaderLib = nullptr;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Unique id of the shader within the shader library, that this material affects.
  //-----------------------------------------------------------------------------------------------------
  std::string m_shaderName;
  //-----------------------------------------------------------------------------------------------------
  /// @brief A pointer to matrices this material should use for the vertex shader.
  //-----------------------------------------------------------------------------------------------------
  std::array<glm::mat4, 3>* m_matrices = nullptr;
  //-----------------------------------------------------------------------------------------------------
  /// @brief A pointer to the camera that is viewing the material.
  //-----------------------------------------------------------------------------------------------------
  std::shared_ptr<Camera> m_cam = nullptr;
};

#endif // MATERIAL_H

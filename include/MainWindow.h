#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DemoScene.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Explcit constructor for the main window. It is explicit so QWidgets can't be implicitly cast
  /// to MainWindows.
  /// @param [io] io_parent the parent widget of this window.
  //-----------------------------------------------------------------------------------------------------
  explicit MainWindow(QWidget *io_parent = nullptr) :
    QMainWindow(io_parent)
  {}
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor
  //-----------------------------------------------------------------------------------------------------
  ~MainWindow() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to initialise the Window with a Scene, placing it in the central widget.
  /// @param [io] io_scene is the scene to be placed in the central widget.
  //-----------------------------------------------------------------------------------------------------
  void init(const std::shared_ptr<Scene> &io_scene);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to handle a key press, will get delegated to the scene.
  /// @param [io] io_event the key that was pressed.
  //-----------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent * io_event);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to handle a mouse move, will get delegated to the scene.
  /// @param [io] io_event contains the mouse position.
  //-----------------------------------------------------------------------------------------------------
  void mouseMoveEvent(QMouseEvent * io_event);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to handle a mouse button press, will get delegated to the scene.
  /// @param [io] io_event contains the mouse button that was pressed.
  //-----------------------------------------------------------------------------------------------------
  void mousePressEvent(QMouseEvent *io_event);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used to handle a mouse button release, will get delegated to the scene.
  /// @param [io] io_event contains the mouse button that was released.
  //-----------------------------------------------------------------------------------------------------
  void mouseReleaseEvent(QMouseEvent *io_event);
  //-----------------------------------------------------------------------------------------------------
  /// @brief The Qt UI form.
  //-----------------------------------------------------------------------------------------------------
  Ui::MainWindow m_ui;
  //-----------------------------------------------------------------------------------------------------
  /// @brief A pointer to the scene that should be placed in the central widget.
  //-----------------------------------------------------------------------------------------------------
  std::shared_ptr<Scene> m_scene = nullptr;

};

#endif // MAINWINDOW_H

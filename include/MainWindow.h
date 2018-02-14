#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DemoScene.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget * parent = nullptr);
  ~MainWindow() = default;

  void init(const std::shared_ptr<Scene> &io_scene);

private:  
  Ui::MainWindow m_ui;
  std::shared_ptr<Scene> m_gl = nullptr;
  void keyPressEvent(QKeyEvent * io_event);
  void mouseMoveEvent(QMouseEvent * io_event);
  void mousePressEvent(QMouseEvent *io_event);
  void mouseReleaseEvent(QMouseEvent *io_event);
};

#endif // MAINWINDOW_H

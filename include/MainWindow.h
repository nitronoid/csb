#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef linux
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#include <QMainWindow>
#include "GLWindow.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(Camera* io_camera, QWidget * parent = nullptr);
  ~MainWindow() = default;

private slots:

private:
  Ui::MainWindow m_ui;

  GLWindow m_gl = {nullptr, this};
  void keyPressEvent(QKeyEvent * _event);
  void mouseMoveEvent(QMouseEvent * _event);
  void mousePressEvent(QMouseEvent *_event);
  void mouseReleaseEvent(QMouseEvent *_event);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OpenglPlatform.h"

#include <QMainWindow>
#include "DemoScene.h"
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

  DemoScene m_gl = {nullptr, this};
  void keyPressEvent(QKeyEvent * _event);
  void mouseMoveEvent(QMouseEvent * _event);
  void mousePressEvent(QMouseEvent *_event);
  void mouseReleaseEvent(QMouseEvent *_event);
};

#endif // MAINWINDOW_H

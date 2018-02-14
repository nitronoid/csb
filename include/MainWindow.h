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
private slots:

private:
  Ui::MainWindow m_ui;

  std::shared_ptr<Scene> m_gl = nullptr;
  void keyPressEvent(QKeyEvent * _event);
  void mouseMoveEvent(QMouseEvent * _event);
  void mousePressEvent(QMouseEvent *_event);
  void mouseReleaseEvent(QMouseEvent *_event);
};

#endif // MAINWINDOW_H

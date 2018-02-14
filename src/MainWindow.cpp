#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  m_ui.setupUi(this);
}

void MainWindow::init(const std::shared_ptr<Scene> &io_scene)
{
  m_gl = io_scene;
  m_ui.s_mainWindowGridLayout->addWidget(m_gl.get(),0,0,3,5);
  connect(m_ui.m_rotating, SIGNAL(clicked(bool)),m_gl.get(), SLOT(rotating(bool)));
  connect(m_ui.generate, SIGNAL( clicked(bool)), m_gl.get(), SLOT(generateNewGeometry()));
  connect(m_ui.material, SIGNAL( clicked(bool)), m_gl.get(), SLOT(nextMaterial()));
}

//----------------------------------------------------------------------------------------------------------------------

void MainWindow::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch ( _event->key() )
  {
    case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
    default : break;
  }
  m_gl->keyPress(_event);
}

//----------------------------------------------------------------------------------------------------------------------

void MainWindow::mouseMoveEvent(QMouseEvent * _event)
{
  m_gl->mouseMove(_event);
}

//----------------------------------------------------------------------------------------------------------------------

void MainWindow::mousePressEvent(QMouseEvent * _event)
{
  m_gl->mouseClick(_event);
}

//----------------------------------------------------------------------------------------------------------------------

void MainWindow::mouseReleaseEvent(QMouseEvent * _event)
{
  m_gl->mouseClick(_event);
}

//----------------------------------------------------------------------------------------------------------------------

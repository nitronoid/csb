
TEMPLATE = app
TARGET = Project

UI_HEADERS_DIR = ui
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui

QT += opengl core gui
CONFIG += console c++14
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += \
  /usr/local/include/glm/glm \
  /usr/local/include/glm \
  $$PWD/include \
  $$PWD/ui \
  $$PWD/shaders

HEADERS += \
  include/MainWindow.h \
  include/GLWindow.h \
  include/Camera.h \
  include/TrackballCamera.h \
  include/Shader.h \
  include/Mesh.h \
  include/Buffer.h \
  include/CameraStates.h \
    include/openglvariadic.h

SOURCES += \
  src/main.cpp \
  src/MainWindow.cpp \
  src/GLWindow.cpp \
  src/Camera.cpp \
  src/TrackballCamera.cpp \
  src/Shader.cpp \
  src/Mesh.cpp \
  src/Buffer.cpp \
  src/CameraStates.cpp

OTHER_FILES += \
  $$files(shaders/*, true) \
  $$files(models/*, true)

FORMS += ui/mainwindow.ui

linux:{
  LIBS += -lGL -lGLU -lGLEW
}


#DISTFILES +=

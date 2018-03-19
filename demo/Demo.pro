TEMPLATE = app
TARGET = Demo

UI_HEADERS_DIR = ui
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui

QT += opengl core gui
CONFIG += console c++14
CONFIG -= app_bundle

DEPENDPATH += . ../csb/lib
INCLUDEPATH += ../csb/include

INCLUDEPATH += \
    /usr/local/include/glm/glm \
    /usr/local/include/glm \
    /usr/local/include \
    $$PWD/include \
    $$PWD/ui \
    $$PWD/shaders

HEADERS += \
    include/MainWindow.h \
    include/Camera.h \
    include/TrackballCamera.h \
    include/CameraStates.h \
    include/Material.h \
    include/Scene.h \
    include/DemoScene.h \
    include/ShaderLib.h \
    include/MeshVBO.h \
    include/MaterialWireframe.h \
    include/MaterialFractal.h \
    include/MaterialCSBpbr.h 

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/Camera.cpp \
    src/TrackballCamera.cpp \
    src/CameraStates.cpp \
    src/Material.cpp \
    src/Scene.cpp \
    src/DemoScene.cpp \
    src/ShaderLib.cpp \
    src/MeshVBO.cpp \
    src/MaterialWireframe.cpp \
    src/MaterialFractal.cpp \
    src/MaterialCSBpbr.cpp 

DISTFILES += $$files(shaders/*, true)

OTHER_FILES += \
    $$files(shaderPrograms/*, true) \
    $$files(models/*, true)

FORMS += ui/mainwindow.ui

QMAKE_CXXFLAGS += -O3 -std=c++14 -msse -msse2 -msse3

LIBS += -L../csb/lib -lcsb

linux:{
    LIBS += -lGL -lGLU -lGLEW -lassimp
}

mac:{
  LIBS+= -L/usr/local/lib -lassimp
  QMAKE_CXXFLAGS += -arch x86_64
}

unix:{
    # add the lib to rpath so it can be dynamically loaded
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/../csb/lib\'"
}


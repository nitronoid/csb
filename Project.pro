TEMPLATE = app
TARGET = Project

UI_HEADERS_DIR = ui
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui

QT += opengl core gui
CONFIG += console c++14
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -O3

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
    include/Camera.h \
    include/TrackballCamera.h \
    include/CameraStates.h \
    include/Material.h \
    include/MaterialPBR.h \
    include/Mesh.h \
    include/Scene.h \
    include/MaterialPhong.h \
    include/ShaderLib.h \
    include/MeshVBO.h \
    include/MaterialWireframe.h \
    include/MaterialFractal.h \
    include/CSBscene.h \
    include/CSBmesh.h \
    include/MaterialEnvMap.h \
    include/CSBconstraint.h \
    include/MaterialCSBpbr.h \
    include/CSBsolver.h \
    include/CSBparticle.h

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/Camera.cpp \
    src/TrackballCamera.cpp \
    src/CameraStates.cpp \
    src/Mesh.cpp \
    src/Material.cpp \
    src/MaterialPBR.cpp \
    src/Scene.cpp \
    src/MaterialPhong.cpp \
    src/ShaderLib.cpp \
    src/MeshVBO.cpp \
    src/MaterialWireframe.cpp \
    src/MaterialFractal.cpp \
    src/CSBscene.cpp \
    src/CSBmesh.cpp \
    src/MaterialEnvMap.cpp \
    src/CSBconstraint.cpp \
    src/MaterialCSBpbr.cpp \
    src/CSBsolver.cpp

OTHER_FILES += \
    $$files(shaders/*, true) \
    $$files(shaderPrograms/*, true) \
    $$files(models/*, true)

FORMS += ui/mainwindow.ui

linux:{
    LIBS += -lGL -lGLU -lGLEW -lassimp
}


#DISTFILES +=

DISTFILES += \
    shaders/WireframeGeometry.glsl \
    shaders/WireframeVertex.glsl \
    shaders/WireframeFragment.glsl \
    shaders/csb_hard_normals.glsl \
    shaders/csb_pbr_vertex.glsl \
    shaders/csb_pbr_frag.glsl

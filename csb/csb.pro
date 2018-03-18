TEMPLATE = lib
TARGET = csb


SOURCES += \
    src/TriMesh.cpp \
    src/SimulatedMesh.cpp \
    src/Solver.cpp \
    src/DistanceConstraint.cpp \
    src/BendingConstraint.cpp \
    src/PinConstraint.cpp \
    src/PositionConstraint.cpp \
    src/StaticCollisionConstraint.cpp \
    src/SphereCollisionConstraint.cpp \
    src/SpatialHash.cpp \
    src/ContinuousCollisionConstraint.cpp \
    src/SelfCollisionSpheresConstraint.cpp \
    src/SelfCollisionRaysConstraint.cpp

HEADERS += \
    include/TriMesh.h \
    include/SimulatedMesh.h \
    include/Particle.h \
    include/Solver.h \
    include/Edge.h \
    include/DistanceConstraint.h \
    include/BendingConstraint.h \
    include/PinConstraint.h \
    include/PositionConstraint.h \
    include/StaticCollisionConstraint.h \
    include/SphereCollisionConstraint.h \
    include/SpatialHash.h \
    include/ContinuousCollisionConstraint.h \
    include/SelfCollisionSpheresConstraint.h \
    include/SelfCollisionRaysConstraint.h

INCLUDEPATH += \
    /usr/local/include/glm/glm \
    /usr/local/include/glm \
    $$PWD/include 

DEFINES += CSB

UI_HEADERS_DIR = ui
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui

QT += opengl core gui
CONFIG += console c++14
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -O3

linux:{
    LIBS += -lGL -lGLU -lGLEW -lassimp
}

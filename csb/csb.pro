TEMPLATE = lib
TARGET = csb

DEFINES += CSB

OBJECTS_DIR = obj
UI_DIR = ui

QT += opengl core gui
CONFIG += console c++14
CONFIG -= app_bundle
DESTDIR = $$PWD/lib

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
    /usr/local/include \
    $$PWD/include 



QMAKE_CXXFLAGS += -O3 -std=c++14 -msse -msse2 -msse3

linux:{
    LIBS += -lGL -lGLU -lGLEW -lassimp
}

mac:{
  LIBS+= -L/usr/local/lib -lassimp
  QMAKE_CXXFLAGS += -arch x86_64
}

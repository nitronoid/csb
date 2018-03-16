TEMPLATE = lib
TARGET = csb


SOURCES += \
    src/Constraint.cpp \
    src/TriMesh.cpp \
    src/SimulatedMesh.cpp \
    src/Solver.cpp

HEADERS += \
    include/Constraint.h \
    include/TriMesh.h \
    include/SimulatedMesh.h \
    include/Particle.h \
    include/Solver.h

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
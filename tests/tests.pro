TEMPLATE = app
TARGET = CSBTests

OBJECTS_DIR = obj

QT += opengl core gui
CONFIG += console c++14
CONFIG -= app_bundle

HEADERS += \
    src/UtilMacros.h

SOURCES += \
    src/main.cpp \
    src/ParticleTests.cpp \
    src/TriMeshTests.cpp \
    src/SimulatedMeshTests.cpp \
    src/DistanceConstraintTests.cpp \
    src/BendingConstraintTests.cpp \
    src/PinConstraintTests.cpp \
    src/EdgeTests.cpp \
    src/SpatialHashTests.cpp \
    src/SphereCollisionConstraintTests.cpp \
    src/SolverTests.cpp \
    src/SelfCollisionSpheresTests.cpp \
    src/SelfCollisionRaysTests.cpp

DEPENDPATH += . ../csb
INCLUDEPATH = ../csb/include
INCLUDEPATH += \
    /usr/local/include/glm/glm \
    /usr/local/include/glm \
    /usr/local/include


QMAKE_CXXFLAGS += -O3 -std=c++14 -msse -msse2 -msse3

LIBS += -L../csb/lib -lcsb -lgtest -pthread

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



TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

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
    src/SelfCollisionSpheresTests.cpp

DEPENDPATH += . ../csb
INCLUDEPATH = ../csb/include
INCLUDEPATH += \
    /usr/local/include/glm/glm \
    /usr/local/include/glm

linux:LIBS += -lgtest -pthread -lassimp -L../csb -lcsb

mac:LIBS+= -L/usr/local/lib -lassimp -L../csb -lcsb

QMAKE_CXXFLAGS += -O3 -std=c++14

HEADERS += \
    src/UtilMacros.h

DISTFILES +=

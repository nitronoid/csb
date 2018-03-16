TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/ParticleTests.cpp \
    src/TriMeshTests.cpp \
    src/SimulatedMeshTests.cpp

DEPENDPATH += . ../csb
INCLUDEPATH = ../csb/include
INCLUDEPATH += \
    /usr/local/include/glm/glm \
    /usr/local/include/glm

LIBS += -L/usr/include/gtest -lgtest -pthread -L../csb -lcsb

QMAKE_CXXFLAGS += -O0

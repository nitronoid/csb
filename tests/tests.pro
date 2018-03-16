TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

SOURCES += tests.cpp

DEPENDPATH += . ../csb
INCLUDEPATH = ../csb/include
INCLUDEPATH += \
    /usr/local/include/glm/glm \
    /usr/local/include/glm

LIBS += -lgtest -lgmock -pthread -L../csb -lcsb

QMAKE_CXXFLAGS += -O3

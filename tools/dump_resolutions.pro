include(../cameraplus.pri)

TEMPLATE = app

DEPENDPATH +=  . ../lib
INCLUDEPATH += . ../lib

LIBS += -L../lib/ -lqtcamera

SOURCES += dump_resolutions.cpp

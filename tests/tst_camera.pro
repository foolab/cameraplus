include(../cameraplus.pri)

TEMPLATE = app
QT += testlib

qt4:QT += declarative
qt5:QT += qml quick

CONFIG += link_pkgconfig
harmattan:PKGCONFIG += gstreamer-0.10
sailfish:PKGCONFIG += gstreamer-1.0

DEPENDPATH += ../declarative ../lib
INCLUDEPATH += ../declarative ../lib

LIBS += -L../lib/ -lqtcamera -L../declarative/ -ldeclarativeqtcamera -Wl,-rpath=/usr/lib/qt4/imports/QtCamera/

SOURCES += tst_camera.cpp
HEADERS += tst_camera.h

TEMPLATE = app
TARGET = cameraplus
DEPENDPATH += . ../
INCLUDEPATH += . ../

QT += declarative opengl

CONFIG += link_pkgconfig debug static

PKGCONFIG = gstreamer-0.10 gstreamer-interfaces-0.10 gstreamer-video-0.10 gstreamer-tag-0.10 \
            gstreamer-pbutils-0.10 meego-gstreamer-interfaces-0.10

LIBS +=  -L../imports/ -limports -L../lib/ -lqtcamera

SOURCES += main.cpp settings.cpp filenaming.cpp
HEADERS += settings.h filenaming.h

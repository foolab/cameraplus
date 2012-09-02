TEMPLATE = lib
TARGET = qtcamera
DEPENDPATH += .
INCLUDEPATH += .

QT += opengl

CONFIG += link_pkgconfig debug

PKGCONFIG = gstreamer-0.10 gstreamer-interfaces-0.10 gstreamer-video-0.10 gstreamer-tag-0.10 \
            gstreamer-pbutils-0.10 meego-gstreamer-interfaces-0.10

HEADERS += qtcamconfig.h qtcamera.h qtcamscanner.h qtcamdevice.h qtcamviewfinder.h \
           qtcammode.h qtcamgstreamermessagehandler.h qtcamgstreamermessagelistener.h \
           qtcamgraphicsviewfinder.h qtcamviewfinderrenderer.h \
           qtcamviewfinderrenderergeneric.h qtcamimagesettings.h qtcamvideosettings.h \
           qtcamimagemode.h qtcamvideomode.h qtcammetadata.h qtcamcapability.h \
           qtcamzoom.h qtcamflash.h qtcamscene.h qtcamevcomp.h qtcamvideotorch.h \
           qtcamwhitebalance.h

SOURCES += qtcamconfig.cpp qtcamera.cpp qtcamscanner.cpp qtcamdevice.cpp qtcamviewfinder.cpp \
           qtcammode.cpp qtcamgstreamermessagehandler.cpp qtcamgstreamermessagelistener.cpp \
           qtcamgraphicsviewfinder.cpp qtcamviewfinderrenderer.cpp \
           qtcamviewfinderrenderergeneric.cpp qtcamimagesettings.cpp qtcamvideosettings.cpp \
           qtcamimagemode.cpp qtcamvideomode.cpp qtcammetadata.cpp qtcamcapability.cpp \
           qtcamzoom.cpp qtcamflash.cpp qtcamscene.cpp qtcamevcomp.cpp qtcamvideotorch.cpp \
           qtcamwhitebalance.cpp

HEADERS += qtcammode_p.h qtcamdevice_p.h qtcamcapability_p.h

LIBS += -lgstphotography-0.10

isEqual(MEEGO_EDITION, harmattan) {
SOURCES += qtcamviewfinderrenderermeego.cpp
HEADERS += qtcamviewfinderrenderermeego.h
}

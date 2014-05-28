TEMPLATE = lib
TARGET = qtcamera
DEPENDPATH += .
INCLUDEPATH += .

include(../cameraplus.pri)

QT += opengl

CONFIG += link_pkgconfig

harmattan:PKGCONFIG = gstreamer-0.10 gstreamer-interfaces-0.10 gstreamer-video-0.10 \
                      gstreamer-tag-0.10 gstreamer-pbutils-0.10 meego-gstreamer-interfaces-0.10 \
                      QtSystemInfo

sailfish:PKGCONFIG = gstreamer-1.0 gstreamer-video-1.0 gstreamer-tag-1.0 \
                     gstreamer-pbutils-1.0 nemo-gstreamer-interfaces-1.0 nemo-gstreamer-meta-1.0 \
                     Qt5SystemInfo

HEADERS += qtcamconfig.h qtcamera.h qtcamscanner.h qtcamdevice.h qtcamviewfinder.h \
           qtcammode.h qtcamgstreamermessagehandler.h qtcamgstreamermessagelistener.h \
           qtcamviewfinderrenderer.h qtcamimagesettings.h qtcamvideosettings.h \
           qtcamimagemode.h qtcamvideomode.h qtcammetadata.h qtcamcapability.h \
           qtcamzoom.h qtcamflash.h qtcamscene.h qtcamevcomp.h qtcamvideotorch.h \
           qtcamwhitebalance.h qtcamcolortone.h qtcamflickerreduction.h \
           qtcamnoisereduction.h qtcamiso.h qtcamaperture.h qtcamexposure.h \
           qtcamvideomute.h qtcamnotifications.h qtcamfocus.h qtcamautofocus.h \
           qtcampropertysetter.h qtcamroi.h qtcamresolution.h

SOURCES += qtcamconfig.cpp qtcamera.cpp qtcamscanner.cpp qtcamdevice.cpp qtcamviewfinder.cpp \
           qtcammode.cpp qtcamgstreamermessagehandler.cpp qtcamgstreamermessagelistener.cpp \
           qtcamviewfinderrenderer.cpp qtcamimagesettings.cpp qtcamvideosettings.cpp \
           qtcamimagemode.cpp qtcamvideomode.cpp qtcammetadata.cpp qtcamcapability.cpp \
           qtcamzoom.cpp qtcamflash.cpp qtcamscene.cpp qtcamevcomp.cpp qtcamvideotorch.cpp \
           qtcamwhitebalance.cpp qtcamcolortone.cpp qtcamflickerreduction.cpp \
           qtcamnoisereduction.cpp qtcamiso.cpp qtcamaperture.cpp qtcamexposure.cpp \
           qtcamvideomute.cpp qtcamnotifications.cpp qtcamfocus.cpp qtcamautofocus.cpp \
           qtcampropertysetter.cpp qtcamroi.cpp qtcamresolution.cpp

HEADERS += qtcammode_p.h qtcamdevice_p.h qtcamcapability_p.h qtcamautofocus_p.h \
           qtcamnotifications_p.h qtcamflash_p.h qtcamroi_p.h

harmattan:LIBS += -lgstphotography-0.10
sailfish:LIBS += -lgstphotography-1.0

harmattan:SOURCES += qtcamviewfinderrenderermeego.cpp
harmattan:HEADERS += qtcamviewfinderrenderermeego.h
sailfish:SOURCES += qtcamviewfinderrenderernemo.cpp
sailfish:HEADERS += qtcamviewfinderrenderernemo.h

qt4:SOURCES += qtcamgraphicsviewfinder.cpp
qt4:HEADERS += qtcamgraphicsviewfinder.h

target.path = /usr/lib/
INSTALLS += target

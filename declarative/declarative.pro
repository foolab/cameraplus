TEMPLATE = lib
TARGET = declarativeqtcamera
DEPENDPATH += . ../lib/
INCLUDEPATH += . ../lib/

CONFIG += link_pkgconfig

PKGCONFIG = gstreamer-pbutils-0.10 libcanberra

LIBS += -L../lib/ -lqtcamera

QT += declarative dbus

HEADERS += plugin.h previewprovider.h camera.h mode.h imagemode.h videomode.h \
           zoom.h flash.h scene.h evcomp.h videotorch.h whitebalance.h \
           colortone.h exposure.h aperture.h iso.h noisereduction.h \
           flickerreduction.h videomute.h metadata.h imagesettings.h \
           imageresolutionmodel.h videosettings.h videoresolutionmodel.h \
           notificationscontainer.h sounds.h focus.h autofocus.h \
           roi.h

SOURCES += plugin.cpp previewprovider.cpp camera.cpp mode.cpp imagemode.cpp videomode.cpp \
           zoom.cpp flash.cpp scene.cpp evcomp.cpp videotorch.cpp whitebalance.cpp \
           colortone.cpp exposure.cpp aperture.cpp iso.cpp noisereduction.cpp \
           flickerreduction.cpp videomute.cpp metadata.cpp imagesettings.cpp \
           imageresolutionmodel.cpp videosettings.cpp videoresolutionmodel.cpp \
           notificationscontainer.cpp sounds.cpp focus.cpp autofocus.cpp \
           roi.cpp

HEADERS += declarativeqtcameranotifications.h

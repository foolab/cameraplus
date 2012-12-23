TEMPLATE = lib
TARGET = imports
DEPENDPATH += . ../lib/
INCLUDEPATH += . ../lib/

CONFIG += link_pkgconfig debug

PKGCONFIG = gstreamer-0.10 gstreamer-interfaces-0.10 gstreamer-video-0.10 gstreamer-tag-0.10 \
            gstreamer-pbutils-0.10 meego-gstreamer-interfaces-0.10 libcanberra

QT += declarative dbus

HEADERS += plugin.h previewprovider.h camera.h mode.h imagemode.h videomode.h \
           zoom.h flash.h scene.h evcomp.h videotorch.h whitebalance.h \
           colortone.h exposure.h aperture.h iso.h noisereduction.h \
           flickerreduction.h videomute.h metadata.h imagesettings.h \
           imageresolutionmodel.h videosettings.h videoresolutionmodel.h \
           notifications.h notificationscontainer.h sounds.h focus.h autofocus.h

SOURCES += plugin.cpp previewprovider.cpp camera.cpp mode.cpp imagemode.cpp videomode.cpp \
           zoom.cpp flash.cpp scene.cpp evcomp.cpp videotorch.cpp whitebalance.cpp \
           colortone.cpp exposure.cpp aperture.cpp iso.cpp noisereduction.cpp \
           flickerreduction.cpp videomute.cpp metadata.cpp imagesettings.cpp \
           imageresolutionmodel.cpp videosettings.cpp videoresolutionmodel.cpp \
           notifications.cpp notificationscontainer.cpp sounds.cpp focus.cpp autofocus.cpp

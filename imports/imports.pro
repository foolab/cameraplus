TEMPLATE = lib
TARGET = imports
DEPENDPATH += . ../lib/
INCLUDEPATH += . ../lib/

CONFIG += link_pkgconfig debug

PKGCONFIG = gstreamer-0.10 gstreamer-interfaces-0.10 gstreamer-video-0.10 gstreamer-tag-0.10 \
            gstreamer-pbutils-0.10 meego-gstreamer-interfaces-0.10

QT += declarative

HEADERS += plugin.h previewprovider.h camera.h mode.h imagemode.h videomode.h \
           capability.h zoom.h flash.h scene.h evcomp.h videotorch.h whitebalance.h \
           colortone.h exposure.h aperture.h iso.h noisereduction.h \
           flickerreduction.h mute.h metadata.h

SOURCES += plugin.cpp previewprovider.cpp camera.cpp mode.cpp imagemode.cpp videomode.cpp \
           capability.cpp zoom.cpp flash.cpp scene.cpp evcomp.cpp videotorch.cpp whitebalance.cpp \
           colortone.cpp exposure.cpp aperture.cpp iso.cpp noisereduction.cpp \
           flickerreduction.cpp mute.cpp metadata.cpp

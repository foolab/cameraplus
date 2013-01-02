TEMPLATE = app
TARGET = cameraplus
DEPENDPATH += . ../
INCLUDEPATH += . ../

QT += declarative opengl dbus

CONFIG += link_pkgconfig debug static mobility qtsparql galleryinterface-maemo-meegotouch \
          shareuiinterface-maemo-meegotouch qmljsdebug

MOBILITY += location systeminfo

PKGCONFIG = gstreamer-0.10 gstreamer-interfaces-0.10 gstreamer-video-0.10 gstreamer-tag-0.10 \
            gstreamer-pbutils-0.10 meego-gstreamer-interfaces-0.10 quill qmsystem2 libresourceqt1

LIBS +=  -L../declarative/ -ldeclarativeqtcamera -L../lib/ -lqtcamera

SOURCES += main.cpp settings.cpp filenaming.cpp quillitem.cpp displaystate.cpp fsmonitor.cpp \
           cameraresources.cpp compass.cpp orientation.cpp geocode.cpp mountprotector.cpp \
           trackerstore.cpp focusrectangle.cpp sharehelper.cpp deletehelper.cpp galleryhelper.cpp \
           postcapturemodel.cpp batteryinfo.cpp gridlines.cpp deviceinfo.cpp devicekeys.cpp

HEADERS += settings.h filenaming.h quillitem.h displaystate.h fsmonitor.h \
           cameraresources.h compass.h orientation.h geocode.h mountprotector.h \
           trackerstore.h focusrectangle.h sharehelper.h deletehelper.h galleryhelper.h \
           postcapturemodel.h batteryinfo.h gridlines.h deviceinfo.h devicekeys.h

RESOURCES += ../qml/qml.qrc

contains(CONFIG, qmljsdebug) {
    message("Enabling QML/JS Debugging")
    include(../qmljsdebugger/qmljsdebugger-src.pri)
}

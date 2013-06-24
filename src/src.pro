TEMPLATE = app
TARGET = cameraplus
DEPENDPATH += . ../
INCLUDEPATH += . ../

QT += declarative opengl dbus

CONFIG += link_pkgconfig mobility qtsparql

MOBILITY += location systeminfo

PKGCONFIG = quill qmsystem2 libresourceqt1

SOURCES += main.cpp settings.cpp filenaming.cpp quillitem.cpp displaystate.cpp fsmonitor.cpp \
           cameraresources.cpp compass.cpp orientation.cpp geocode.cpp mountprotector.cpp \
           trackerstore.cpp focusrectangle.cpp sharehelper.cpp deletehelper.cpp galleryhelper.cpp \
           postcapturemodel.cpp batteryinfo.cpp gridlines.cpp deviceinfo.cpp devicekeys.cpp \
           platformsettings.cpp dbusservice.cpp

HEADERS += settings.h filenaming.h quillitem.h displaystate.h fsmonitor.h \
           cameraresources.h compass.h orientation.h geocode.h mountprotector.h \
           trackerstore.h focusrectangle.h sharehelper.h deletehelper.h galleryhelper.h \
           postcapturemodel.h batteryinfo.h gridlines.h deviceinfo.h devicekeys.h \
           platformsettings.h dbusservice.h

RESOURCES += ../qml/qml.qrc

contains(CONFIG, qmljsdebug) {
    message("Enabling QML/JS Debugging")
    QT += script
    include(../qmljsdebugger/qmljsdebugger-src.pri)
}

target.path = /usr/bin/
INSTALLS += target

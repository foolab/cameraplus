TEMPLATE = app
TARGET = cameraplus
DEPENDPATH += . ../
INCLUDEPATH += . ../
include(../cameraplus.pri)

QT += dbus
CONFIG += link_pkgconfig

qt4 {
    QT += declarative opengl
    CONFIG += mobility
    MOBILITY += location systeminfo
    PKGCONFIG += QtSparql qmsystem2 libresourceqt1 qdeclarative-boostable
}

qt5 {
    QT += qml quick
    PKGCONFIG += Qt5Sparql qmsystem2-qt5 libresourceqt5 qdeclarative5-boostable
}

SOURCES += main.cpp settings.cpp filenaming.cpp displaystate.cpp fsmonitor.cpp \
           cameraresources.cpp compass.cpp orientation.cpp mountprotector.cpp \
           trackerstore.cpp focusrectangle.cpp sharehelper.cpp deletehelper.cpp galleryhelper.cpp \
           postcapturemodel.cpp batteryinfo.cpp gridlines.cpp devicekeys.cpp \
           platformsettings.cpp dbusservice.cpp

HEADERS += settings.h filenaming.h displaystate.h fsmonitor.h \
           cameraresources.h compass.h orientation.h mountprotector.h \
           trackerstore.h focusrectangle.h sharehelper.h deletehelper.h galleryhelper.h \
           postcapturemodel.h batteryinfo.h gridlines.h devicekeys.h \
           platformsettings.h dbusservice.h

RESOURCES += ../qml/qml.qrc

contains(CONFIG, qmljsdebug) {
    message("Enabling QML/JS Debugging")
    QT += script
    include(../qmljsdebugger/qmljsdebugger-src.pri)
}

harmattan {
    include(harmattan/harmattan.pri)
    RESOURCES += ../qml/harmattan/harmattan.qrc
}

nemo {
    include(nemo/nemo.pri)
    RESOURCES += ../qml/harmattan/harmattan.qrc
}

qt4 {
# TODO: geocode.*
    SOURCES += qmlfileengine.cpp geocode.cpp deviceinfo.cpp
    HEADERS += qmlfileengine.h geocode.h deviceinfo.h
}

target.path = /usr/bin/
INSTALLS += target

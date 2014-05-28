TEMPLATE = app
TARGET = cameraplus
DEPENDPATH += . ../
INCLUDEPATH += . ../
include(../cameraplus.pri)

QT += dbus
CONFIG += link_pkgconfig

qt4 {
    QT += declarative opengl
    PKGCONFIG += QtSparql qmsystem2 libresourceqt1 qdeclarative-boostable quill \
                 contextsubscriber-1.0
}

qt5 {
    QT += qml quick
    PKGCONFIG += Qt5Sparql libresourceqt5 qdeclarative5-boostable quill-qt5 \
                 contextkit-statefs Qt5Sensors
}

SOURCES += main.cpp \
           settings.cpp filenaming.cpp cameraresources.cpp \
           trackerstore.cpp focusrectangle.cpp sharehelper.cpp \
           deletehelper.cpp galleryhelper.cpp postcapturemodel.cpp \
           gridlines.cpp platformsettings.cpp dbusservice.cpp \
           mountprotector.cpp devicesettings.cpp fileindex.cpp \
           stack.cpp toolbarlayout.cpp quillitem.cpp pluginloader.cpp \
           qrangemodel.cpp phoneprofile.cpp devicefeatures.cpp

HEADERS += settings.h filenaming.h cameraresources.h \
           trackerstore.h focusrectangle.h sharehelper.h \
           deletehelper.h galleryhelper.h postcapturemodel.h \
           gridlines.h platformsettings.h dbusservice.h \
           mountprotector.h devicesettings.h fileindex.h \
           stack.h toolbarlayout.h quillitem.h pluginloader.h \
           qrangemodel.h phoneprofile.h devicefeatures.h

RESOURCES += ../qml/qml.qrc ../icons/icons.qrc

contains(CONFIG, qmljsdebug) {
    message("Enabling QML/JS Debugging")
    QT += script
    include(../qmljsdebugger/qmljsdebugger-src.pri)
}

harmattan {
    include(harmattan/harmattan.pri)
    RESOURCES += ../qml/harmattan/harmattan.qrc
}

sailfish {
    include(sailfish/sailfish.pri)
    RESOURCES += ../qml/sailfish/sailfish.qrc
}

qt4 {
    SOURCES += qmlfileengine.cpp
    HEADERS += qmlfileengine.h
}

target.path = /usr/bin/
INSTALLS += target

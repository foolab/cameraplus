DEPENDPATH += sailfish .
INCLUDEPATH += sailfish .

PKGCONFIG += Qt5SystemInfo Qt5Location

HEADERS += sailfish/soundvolumecontrol.h \
           sailfish/deviceinfo.h \
           sailfish/geocode.h \
           sailfish/displaystate.h \
           sailfish/fsmonitor.h \
           sailfish/compass.h \
           sailfish/orientation.h \
           sailfish/mountprotector.h \
           sailfish/batteryinfo.h \
           sailfish/devicekeys.h

SOURCES += sailfish/soundvolumecontrol.cpp \
           sailfish/deviceinfo.cpp \
           sailfish/geocode.cpp \
           sailfish/displaystate.cpp \
           sailfish/fsmonitor.cpp \
           sailfish/compass.cpp \
           sailfish/orientation.cpp \
           sailfish/mountprotector.cpp \
           sailfish/batteryinfo.cpp \
           sailfish/devicekeys.cpp

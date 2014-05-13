DEPENDPATH += harmattan .
INCLUDEPATH += harmattan .

PKGCONFIG += QtLocation QtSystemInfo locationextras

HEADERS += soundvolumecontrol.h deviceinfo.h geocode.h \
	   batteryinfo.h compass.h devicekeys.h \
	   displaystate.h fsmonitor.h orientation.h \
           platformquirks.h proximity.h

SOURCES += soundvolumecontrol.cpp deviceinfo.cpp geocode.cpp \
	   batteryinfo.cpp compass.cpp devicekeys.cpp \
	   displaystate.cpp fsmonitor.cpp orientation.cpp \
           platformquirks.cpp proximity.cpp

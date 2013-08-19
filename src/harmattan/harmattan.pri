DEPENDPATH += harmattan .
INCLUDEPATH += harmattan .

PKGCONFIG += quill contextsubscriber-1.0 QtLocation QtSystemInfo

HEADERS += quillitem.h soundvolumecontrol.h deviceinfo.h geocode.h \
	   batteryinfo.h compass.h devicekeys.h \
	   displaystate.h fsmonitor.h orientation.h phoneprofile.h

SOURCES += quillitem.cpp soundvolumecontrol.cpp deviceinfo.cpp geocode.cpp \
	   batteryinfo.cpp compass.cpp devicekeys.cpp \
	   displaystate.cpp fsmonitor.cpp orientation.cpp phoneprofile.cpp

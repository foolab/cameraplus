set(EXTRA_SOURCE_FILES
  sailfish/compass.cpp
  sailfish/devicekeys.cpp
  sailfish/fsmonitor.cpp
  sailfish/orientation.cpp
  sailfish/deviceinfo.cpp
  sailfish/displaystate.cpp
  sailfish/geocode.cpp
  sailfish/soundvolumecontrol.cpp
  )

set(EXTRA_HEADER_FILES
  sailfish/compass.h
  sailfish/devicekeys.h
  sailfish/fsmonitor.h
  sailfish/orientation.h
  sailfish/deviceinfo.h
  sailfish/displaystate.h
  sailfish/geocode.h
  sailfish/soundvolumecontrol.h
  )

pkg_check_modules(EXTRA REQUIRED
  Qt5SystemInfo
  Qt5Sparql
  libresourceqt5
  qdeclarative5-boostable
  quill-qt5
  contextkit-statefs
  Qt5Sensors
  )

set(EXTRA_RESOURCES "../qml/sailfish/sailfish.qrc")

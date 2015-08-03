set(EXTRA_SOURCE_FILES
  harmattan/soundvolumecontrol.cpp
  harmattan/deviceinfo.cpp
  harmattan/geocode.cpp
  harmattan/compass.cpp
  harmattan/devicekeys.cpp
  harmattan/displaystate.cpp
  harmattan/fsmonitor.cpp
  harmattan/orientation.cpp
  harmattan/platformquirks.cpp
  harmattan/proximity.cpp
  qmlfileengine.cpp
  )

set(EXTRA_HEADER_FILES
  harmattan/soundvolumecontrol.h
  harmattan/deviceinfo.h
  harmattan/geocode.h
  harmattan/compass.h
  harmattan/devicekeys.h
  harmattan/displaystate.h
  harmattan/fsmonitor.h
  harmattan/orientation.h
  harmattan/platformquirks.h
  harmattan/proximity.h
  qmlfileengine.h
  )

pkg_check_modules(EXTRA REQUIRED
  QtLocation
  QtSystemInfo
  locationextras
  qmsystem2
  qdeclarative-boostable
  quill
  contextsubscriber-1.0
  QtSparql
  libresourceqt1
  )

set(EXTRA_RESOURCES "../qml/harmattan/harmattan.qrc")

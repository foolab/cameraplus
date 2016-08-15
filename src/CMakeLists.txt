set(SOURCE_FILES
  main.cpp
  settings.cpp
  filenaming.cpp
  cameraresources.cpp
  trackerstore.cpp
  focusrectangle.cpp
  sharehelper.cpp
  deletehelper.cpp
  galleryhelper.cpp
  postcapturemodel.cpp
  gridlines.cpp
  platformsettings.cpp
  dbusservice.cpp
  mountprotector.cpp
  devicesettings.cpp
  fileindex.cpp
  stack.cpp
  toolbarlayout.cpp
  quillitem.cpp
  pluginloader.cpp
  qrangemodel.cpp
  phoneprofile.cpp
  devicefeatures.cpp
  position.cpp
  batteryinfo.cpp
  )

set(HEADER_FILES
  settings.h
  filenaming.h
  cameraresources.h
  trackerstore.h
  focusrectangle.h
  sharehelper.h
  deletehelper.h
  galleryhelper.h
  postcapturemodel.h
  gridlines.h
  platformsettings.h
  dbusservice.h
  mountprotector.h
  devicesettings.h
  fileindex.h
  stack.h
  toolbarlayout.h
  quillitem.h
  pluginloader.h
  qrangemodel.h
  phoneprofile.h
  devicefeatures.h
  position.h
  batteryinfo.h
  )

if (HARMATTAN)
  include_directories(harmattan)
  include(harmattan/CMakeLists.txt)

  qt4_add_resources(qrc_extra ${EXTRA_RESOURCES})
  qt4_add_resources(qrc_qml ../qml/qml.qrc)
  qt4_add_resources(qrc_icons ../icons/icons.qrc)
elseif (SAILFISH)
  include_directories(sailfish)
  include(sailfish/CMakeLists.txt)

  qt5_add_resources(qrc_extra ${EXTRA_RESOURCES})
  qt5_add_resources(qrc_qml ../qml/qml.qrc)
  qt5_add_resources(qrc_icons ../icons/icons.qrc)

  # external targets
  include_directories("${CMAKE_BINARY_DIR}/deps/usr/include/qt5/quill-qt5/")
endif()

qt_wrap_cpp(MOC_FILES ${HEADER_FILES} ${EXTRA_HEADER_FILES})

add_executable(cameraplus
  ${SOURCE_FILES} ${MOC_FILES}
  ${EXTRA_SOURCE_FILES}
  ${qrc_extra} ${qrc_qml} ${qrc_icons}
  )

add_dependencies(cameraplus qtcamera translations)

add_definitions(${EXTRA_CFLAGS})
include_directories(${EXTRA_INCLUDE_DIRS})

target_link_libraries(cameraplus ${EXTRA_LDFLAGS})
target_link_libraries(cameraplus ${QT_LIBRARIES})

if (SAILFISH)
  qt5_use_modules(cameraplus Gui Quick)
  add_dependencies(cameraplus quill)
  target_link_libraries(cameraplus -L${CMAKE_BINARY_DIR}/deps/usr/lib/ -lquill-qt5 -lffmpegthumbnailer -L${FFMPEG_DIR}/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale -lexempi)
endif()

install(TARGETS cameraplus DESTINATION bin)

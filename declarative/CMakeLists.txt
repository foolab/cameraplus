set(SOURCE_FILES
  plugin.cpp
  previewprovider.cpp
  camera.cpp
  mode.cpp
  imagemode.cpp
  videomode.cpp
  zoom.cpp
  flash.cpp
  scene.cpp
  evcomp.cpp
  videotorch.cpp
  whitebalance.cpp
  colortone.cpp
  exposure.cpp
  aperture.cpp
  iso.cpp
  noisereduction.cpp
  flickerreduction.cpp
  videomute.cpp
  metadata.cpp
  imagesettings.cpp
  resolutionmodel.cpp
  videosettings.cpp
  notificationscontainer.cpp
  sounds.cpp
  focus.cpp
  autofocus.cpp
  roi.cpp
  cameraconfig.cpp
  videoplayer.cpp
  viewfinder.cpp
  capability.cpp
  resolution.cpp
  viewfinderbufferhandler.cpp
  viewfinderframehandler.cpp
  viewfinderhandler.cpp
  )

set(HEADER_FILES
  plugin.h
  previewprovider.h
  camera.h
  mode.h
  imagemode.h
  videomode.h
  zoom.h
  flash.h
  scene.h
  evcomp.h
  videotorch.h
  whitebalance.h
  colortone.h
  exposure.h
  aperture.h
  iso.h
  noisereduction.h
  flickerreduction.h
  videomute.h
  metadata.h
  imagesettings.h
  resolutionmodel.h
  videosettings.h
  notificationscontainer.h
  sounds.h
  focus.h
  autofocus.h
  roi.h
  cameraconfig.h
  videoplayer.h
  viewfinder.h
  capability.h
  resolution.h
  viewfinderbufferhandler.h
  viewfinderframehandler.h
  viewfinderhandler.h
  )

qt_wrap_cpp(MOC_FILES ${HEADER_FILES})

pkg_check_modules(DECLARATIVE REQUIRED sndfile libpulse)

add_library(declarativeqtcamera MODULE ${SOURCE_FILES} ${MOC_FILES})

add_definitions(${DECLARATIVE_INCLUDE_CFLAGS})
include_directories(../lib/ ${DECLARATIVE_INCLUDE_DIRS})
configure_gst(declarativeqtcamera)

target_link_libraries(declarativeqtcamera ${DECLARATIVE_LIBRARIES} qtcamera)

install(TARGETS declarativeqtcamera DESTINATION ${QT_INSTALL_IMPORTS}/QtCamera)
install(FILES qmldir DESTINATION ${QT_INSTALL_IMPORTS}/QtCamera)

add_definitions(-DQT_PLUGIN)

if (SAILFISH)
  qt5_use_modules(declarativeqtcamera Qml Quick DBus)
endif()

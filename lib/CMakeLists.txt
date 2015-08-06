set(SOURCE_FILES
  qtcamconfig.cpp
  qtcamera.cpp
  qtcamscanner.cpp
  qtcamdevice.cpp
  qtcamviewfinder.cpp
  qtcammode.cpp
  qtcamgstmessagehandler.cpp
  qtcamgstmessagelistener.cpp
  qtcamviewfinderrenderer.cpp
  qtcamimagesettings.cpp
  qtcamvideosettings.cpp
  qtcamimagemode.cpp
  qtcamvideomode.cpp
  qtcammetadata.cpp
  qtcamcapability.cpp
  qtcamzoom.cpp
  qtcamflash.cpp
  qtcamscene.cpp
  qtcamevcomp.cpp
  qtcamvideotorch.cpp
  qtcamwhitebalance.cpp
  qtcamcolortone.cpp
  qtcamflickerreduction.cpp
  qtcamnoisereduction.cpp
  qtcamiso.cpp
  qtcamaperture.cpp
  qtcamexposure.cpp
  qtcamvideomute.cpp
  qtcamnotifications.cpp
  qtcamfocus.cpp
  qtcamautofocus.cpp
  qtcampropertysetter.cpp
  qtcamroi.cpp
  qtcamresolution.cpp
  qtcamviewfinderbufferlistener.cpp
  qtcamviewfinderbufferhandler.cpp
  qtcamgstsample.cpp
  qtcamnullviewfinder.cpp
  qtcamutils.cpp
  qtcamviewfinderframe.cpp
  qtcamviewfinderframehandler.cpp
  qtcamviewfinderframelistener.cpp
  )

set(HEADER_FILES
  qtcamconfig.h
  qtcamera.h
  qtcamscanner.h
  qtcamdevice.h
  qtcamviewfinder.h
  qtcammode.h
  qtcamgstmessagehandler.h
  qtcamgstmessagelistener.h
  qtcamviewfinderrenderer.h
  qtcamimagesettings.h
  qtcamvideosettings.h
  qtcamimagemode.h
  qtcamvideomode.h
  qtcammetadata.h
  qtcamcapability.h
  qtcamzoom.h
  qtcamflash.h
  qtcamscene.h
  qtcamevcomp.h
  qtcamvideotorch.h
  qtcamwhitebalance.h
  qtcamcolortone.h
  qtcamflickerreduction.h
  qtcamnoisereduction.h
  qtcamiso.h
  qtcamaperture.h
  qtcamexposure.h
  qtcamvideomute.h
  qtcamnotifications.h
  qtcamfocus.h
  qtcamautofocus.h
  qtcampropertysetter.h
  qtcamroi.h
  qtcamresolution.h
  qtcamviewfinderbufferlistener.h
  qtcamviewfinderbufferhandler.h
  qtcamgstsample.h
  qtcamnullviewfinder.h
  qtcamutils.h
  qtcamviewfinderframe.h
  qtcamviewfinderframehandler.h
  qtcamviewfinderframelistener.h
  )

set(PRIVATE_HEADER_FILES
  qtcammode_p.h
  qtcamdevice_p.h
  qtcamcapability_p.h
  qtcamautofocus_p.h
  qtcamnotifications_p.h
  qtcamflash_p.h
  qtcamroi_p.h
  qtcamviewfinderbufferlistener_p.h
  qtcamconfig_p.h
  qtcamviewfinderrenderer_p.h
  qtcamviewfinderframelistener_p.h
  qtcamvideomode_p.h
  )

if (HARMATTAN)
  set(EXTRA_HEADER_FILES qtcamviewfinderrenderermeego.h qtcamgraphicsviewfinder.h)
  set(EXTRA_SOURCE_FILES qtcamviewfinderrenderermeego.cpp qtcamgraphicsviewfinder.cpp)
  pkg_check_modules(GST REQUIRED gstreamer-0.10 gstreamer-interfaces-0.10 gstreamer-video-0.10
    gstreamer-tag-0.10 gstreamer-pbutils-0.10 meego-gstreamer-interfaces-0.10)

  # We need to set this manually
  set(EXTRA_LIBRARIES -lgstphotography-0.10)
elseif (SAILFISH)
  set(EXTRA_HEADER_FILES qtcamviewfinderrenderernemo.h)
  set(EXTRA_SOURCE_FILES qtcamviewfinderrenderernemo.cpp)
  pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-video-1.0 gstreamer-tag-1.0
    gstreamer-pbutils-1.0 nemo-gstreamer-interfaces-1.0 nemo-gstreamer-meta-1.0)

  # We need to set this manually
  set(EXTRA_LIBRARIES -lgstphotography-1.0)
endif()

qt_wrap_cpp(MOC_FILES ${HEADER_FILES} ${PRIVATE_HEADER_FILES} ${EXTRA_HEADER_FILES})
add_library(qtcamera SHARED ${SOURCE_FILES} ${EXTRA_SOURCE_FILES} ${MOC_FILES})

if (QT5)
  qt5_use_modules(qtcamera Gui)
endif()

add_definitions(${QT_SYSTEM_INFO_CFLAGS} ${GST_INCLUDE_CFLAGS})
include_directories(${QT_SYSTEM_INFO_INCLUDE_DIRS} ${GST_INCLUDE_DIRS})
target_link_libraries(qtcamera ${GST_LIBRARIES} ${QT_SYSTEM_INFO_LIBRARIES} ${EXTRA_LIBRARIES})

if (HARMATTAN)
  target_link_libraries(qtcamera Qt4::QtOpenGL)
elseif (SAILFISH)
  # TODO:
endif()

set_property(TARGET qtcamera PROPERTY VERSION "1.0.0")
set_property(TARGET qtcamera PROPERTY SOVERSION 1)

install(TARGETS qtcamera DESTINATION lib)

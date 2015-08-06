set(SOURCE_FILES
  panoramaplugin.cpp
  panorama.cpp
  panoramatracker.cpp
  panoramastitcher.cpp
  panoramainput.cpp
  )

set(HEADER_FILES
  panoramaplugin.h
  panorama.h
  panoramatracker.h
  panoramastitcher.h
  panoramainput.h
  )

qt_wrap_cpp(MOC_FILES ${HEADER_FILES})

link_directories(${LIBYUV_LIB_DIR} ${PANORAMA_STITCHER_LIB_DIR})
add_library(cameraplus-panorama MODULE ${SOURCE_FILES} ${MOC_FILES})
target_link_libraries(cameraplus-panorama ${DECLARATIVE_LIBRARIES} qtcamera yuv panorama-stitcher)
include_directories(../lib/
  ${LIBYUV_INCLUDE_DIR}
  ${PANORAMA_STITCHER_INCLUDE_DIR}
  )

configure_gst(cameraplus-panorama)
add_dependencies(cameraplus-panorama libyuv panorama-stitcher)
install(TARGETS cameraplus-panorama DESTINATION lib)

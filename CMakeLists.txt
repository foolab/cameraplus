cmake_minimum_required(VERSION 2.8.2)
project(CameraPlus)

MACRO(QT_WRAP_CPP ...)
  if (QT4)
    QT4_WRAP_CPP(${ARGV})
  elseif(QT5)
    QT5_WRAP_CPP(${ARGV})
  endif()
ENDMACRO(QT_WRAP_CPP)

MACRO(CONFIGURE_GST target)
  if (HARMATTAN)
    pkg_check_modules(GST REQUIRED gstreamer-0.10)
  elseif (SAILFISH)
    pkg_check_modules(GST REQUIRED gstreamer-1.0)
  endif()

  add_definitions(${GST_INCLUDE_CFLAGS})
  include_directories(${GST_INCLUDE_DIRS})
  target_link_libraries(${target} ${GST_LIBRARIES})
ENDMACRO(CONFIGURE_GST)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
set(CUSTOM_DATA_DIR "" CACHE FILEPATH "Custom DATA_DIR")

if (DATA_DIR)
  add_definitions(-DDATA_DIR="${DATA_DIR}")
endif()

if (TRANSLATIONS_DIR)
  add_definitions(-DTRANSLATIONS_DIR="${TRANSLATIONS_DIR}")
endif()

find_package(PkgConfig)

if (PLATFORM STREQUAL "sailfish")
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Qml REQUIRED)
  find_package(Qt5Quick REQUIRED)
  find_package(Qt5Qml REQUIRED)
  find_package(Qt5LinguistTools REQUIRED)
  pkg_check_modules(QT_SYSTEM_INFO REQUIRED Qt5SystemInfo)
  add_definitions(-DQT5 -DSAILFISH)
  set(SAILFISH 1)
  set(QT5 1)

  # This is used by exec_program() to get the imports dir
  get_target_property(QT_QMAKE_EXECUTABLE Qt5::qmake LOCATION)
  # We need to query this manually
  exec_program(${QT_QMAKE_EXECUTABLE}
    ARGS "-query QT_INSTALL_QML " OUTPUT_VARIABLE QT_INSTALL_IMPORTS)

  message("Building for Sailfish")
elseif (PLATFORM STREQUAL "harmattan")
  find_package(Qt4 REQUIRED QtCore QtGui QtDeclarative QtDbus QtOpengl)
  pkg_check_modules(QT_SYSTEM_INFO REQUIRED QtSystemInfo)
  add_definitions(-DQT4 -DHARMATTAN)
  set(HARMATTAN 1)
  set(QT4 1)
  include(${QT_USE_FILE})

  # We need to query this manually
  exec_program(${QT_QMAKE_EXECUTABLE}
    ARGS "-query QT_INSTALL_IMPORTS " OUTPUT_VARIABLE QT_INSTALL_IMPORTS)

  message("Building for Harmattan")
else()
  message(FATAL_ERROR "unknown platform")
endif()


add_subdirectory(external)

ExternalProject_Get_Property(libyuv source_dir)
set(LIBYUV_INCLUDE_DIR ${source_dir}/include)
ExternalProject_Get_Property(libyuv install_dir)
set(LIBYUV_LIB_DIR ${install_dir}/lib)

ExternalProject_Get_Property(libpanorama-stitcher source_dir)
set(PANORAMA_STITCHER_INCLUDE_DIR ${source_dir})
ExternalProject_Get_Property(libpanorama-stitcher install_dir)
set(PANORAMA_STITCHER_LIB_DIR ${install_dir}/lib)

add_subdirectory(translations)
add_subdirectory(lib)
add_subdirectory(declarative)
add_subdirectory(src)
add_subdirectory(panorama)
add_subdirectory(tools)
add_subdirectory(tests)

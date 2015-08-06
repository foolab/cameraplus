cmake_minimum_required (VERSION 2.8.2)
project (CameraPlus)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

set(CUSTOM_DATA_DIR "" CACHE FILEPATH "Custom DATA_DIR")

if (DATA_DIR)
  add_definitions(-DDATA_DIR="${DATA_DIR}")
endif()

find_package(PkgConfig)

if (PLATFORM STREQUAL "sailfish")
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Qml REQUIRED)
  find_package(Qt5Quick REQUIRED)
  find_package(Qt5Qml REQUIRED)
  pkg_check_modules(QT_SYSTEM_INFO REQUIRED Qt5SystemInfo)
  add_definitions(-DQT5 -DSAILFISH)
  set(SAILFISH 1)
  message("Building for Sailfish")
elseif (PLATFORM STREQUAL "harmattan")
  find_package(Qt4 REQUIRED QtCore QtGui QtDeclarative QtDbus QtOpengl)
  pkg_check_modules(QT_SYSTEM_INFO REQUIRED QtSystemInfo)
  add_definitions(-DQT4 -DHARMATTAN)
  set(HARMATTAN 1)
  include(${QT_USE_FILE})
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

add_subdirectory(lib)
add_subdirectory(declarative)
add_subdirectory(src)
add_subdirectory(panorama)

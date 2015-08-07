include(ExternalProject)

# snippet stolen from https://gist.github.com/simonlynen/5540117
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
  set(EXTRA_CFLAGS -mfpu=neon)
endif()

ExternalProject_Add(libpanorama-stitcher
  URL ${CMAKE_CURRENT_SOURCE_DIR}/panorama-stitcher
  PATCH_COMMAND NOCONFIGURE=1 ./autogen.sh
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --enable-static --disable-shared --prefix=<INSTALL_DIR> CFLAGS=-fPIC CXXFLAGS=-fPIC\ -ffast-math\ -O2\ ${EXTRA_CFLAGS}
  )

ExternalProject_Add(libyuv
  URL ${CMAKE_CURRENT_SOURCE_DIR}/libyuv
  CONFIGURE_COMMAND /bin/true
  BUILD_COMMAND make -f linux.mk CXXFLAGS=-O2\ -fomit-frame-pointer\ -fPIC\ -Iinclude/\ ${EXTRA_CFLAGS}
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND mkdir -p <INSTALL_DIR>/lib && cp -a <SOURCE_DIR>/libyuv.a <INSTALL_DIR>/lib/
  )

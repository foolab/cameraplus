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

if (SAILFISH)
# ffmpeg
ExternalProject_Add(ffmpeg
  URL ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --disable-static --enable-shared --prefix=${CMAKE_BINARY_DIR}/deps/usr/ --disable-ffmpeg --disable-ffplay --disable-ffprobe --disable-ffserver --disable-doc --enable-pic --disable-devices --disable-network --disable-hwaccels --disable-filters --enable-gpl --disable-bzlib --disable-encoders --disable-muxers --disable-yasm
  BUILD_COMMAND make -j4
  INSTALL_COMMAND make install
  )

# ffmpegthumbnailer
# We execute cmake manually to be able to set PKG_CONFIG_PATH
ExternalProject_Add(ffmpegthumbnailer
  DEPENDS ffmpeg
  URL ${CMAKE_CURRENT_SOURCE_DIR}/ffmpegthumbnailer
  PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_LIST_DIR}/0001-fix-c-11-flags.patch
  CONFIGURE_COMMAND PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/deps/usr/lib/pkgconfig cmake -DENABLE_TESTS=OFF -DENABLE_GIO=OFF -DENABLE_THUMBNAILER=OFF -DENABLE_STATIC=OFF -DENABLE_SHARED=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/deps/usr/ <SOURCE_DIR>
  )

# quill
ExternalProject_Add(quill
  DEPENDS ffmpegthumbnailer
  URL ${CMAKE_CURRENT_SOURCE_DIR}/quill
  PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_LIST_DIR}/quill-build-only-src.patch
  CONFIGURE_COMMAND PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/deps/usr/lib/pkgconfig qmake <SOURCE_DIR>
  BUILD_COMMAND PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/deps/usr/lib/pkgconfig make
  INSTALL_COMMAND make install INSTALL_ROOT=${CMAKE_BINARY_DIR}/deps/
  )

endif(SAILFISH)

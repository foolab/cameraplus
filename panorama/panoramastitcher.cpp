/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "panoramastitcher.h"
#include "panorama.h"
#include <QTimer>
#include <QImage>
#include <QDir>
#include <jpeglib.h>
#include "libyuv.h"
#include "mosaic/Blend.h"
#include <cstdio>
#include <csetjmp>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

#define PROGRESS_TIMER_INTERVAL 500

struct __error_mgr : public jpeg_error_mgr {
  jmp_buf setjmp_buffer;
} __error_mgr;

static void
__error_exit (j_common_ptr cinfo) {
  struct __error_mgr *myerr = (struct __error_mgr *) cinfo->err;

  char buff[JMSG_LENGTH_MAX];
  (*cinfo->err->format_message)(cinfo, buff);

  qWarning() << buff;

  /* jump */
  longjmp(myerr->setjmp_buffer, 1);
}

PanoramaStitcher::PanoramaStitcher(std::vector<guint8 *>& frames, const QSize& size,
				   const QString& output,
				   bool keepFrames, QObject *parent) :
  QThread(parent),
  Stitcher(size.width(), size.height(), frames.size()),
  m_output(output),
  m_running(true),
  m_keepFrames(keepFrames),
  m_alignProgress(0),
  m_size(size) {

  std::swap(m_frames, frames);

  m_timer.setInterval(PROGRESS_TIMER_INTERVAL);
  QObject::connect(&m_timer, SIGNAL(timeout()), this, SIGNAL(progressChanged()));
  m_timer.start();
}

PanoramaStitcher::~PanoramaStitcher() {
  Panorama::clear(m_frames);
}

void PanoramaStitcher::stop() {
  m_running = false;
  cancel();
  wait();
}

void PanoramaStitcher::run() {
  int w, h;
  const unsigned char *im;

  if (m_keepFrames) {
    dumpFrames();
  }

  int size = m_frames.size();
  for (int x = 0; x < size; x++) {
    if (!m_running) {
      goto out;
    }

    // We will ignore error here. We have already added scaled down versions of
    // all the frames to tracker before.
    addFrame(m_frames[x]);

    m_progressLock.lock();
    m_alignProgress = (TIME_PERCENT_ALIGN/size) * x;
    m_progressLock.unlock();

    emit progressChanged();
  }

  if (!m_running) {
    goto out;
  }

  if (stitch() == Stitcher::Error) {
    emit error(Panorama::ErrorStitch);
    goto out;
  }

  if (!m_running) {
    goto out;
  }

  im = image(w, h);

  if (!writeJpeg(im, QSize(w, h), m_output)) {
    emit error(Panorama::ErrorSave);
  }

out:
  Panorama::clear(m_frames);
  emit done();
}

int PanoramaStitcher::progress() {
  QMutexLocker l(&m_progressLock);
  return m_alignProgress + Stitcher::progress();
}

void PanoramaStitcher::dumpFrames() {
  QString d = QString("%1.frames").arg(m_output);
  QDir dir(d);

  if (!dir.mkpath(".")) {
    emit error(Panorama::ErrorIntermediatesDirectory);
    return;
  }

  guint8 rgb[m_size.width() * m_size.height() * 3];

  guint8 *y, *u, *v;
  int width = m_size.width(), height = m_size.height();

  for (int x = 0; x < m_frames.size(); x++) {
    guint8 *data = m_frames[x];

    y = data;
    u = y + width * height;
    v = u + width/2 * height/2;

    // Convert to RGB:
    if (libyuv::I420ToRGB24(y, width,
			    u, width/2,
			    v, width/2,
			    rgb, width * 3,
			    width, height) != 0) {
      emit error(Panorama::ErrorIntermediatesConvert);
      continue;
    }

    // TODO: find a better way
    // We need to convert the bgr we get from libyuv to rgb
    int size = width * height * 3;
    guint8 *s = rgb;
    guint8 a, b;
    for (int i = 0; i < size; i += 3) {
      a = s[0];
      b = s[2];
      s[0] = b;
      s[2] = a;
      s += 3;
    }

    QString file = QString("%1/%2.jpg").arg(d).arg(x);
    if (!writeJpeg(rgb, QSize(width, height), file)) {
      emit error(Panorama::ErrorIntermediatesSave);
      continue;
    }
  }
}

bool PanoramaStitcher::writeJpeg(const guint8 *data, const QSize& size, const QString& fileName) {
  struct jpeg_compress_struct cinfo;
  struct __error_mgr jerr;

  FILE *outFile = fopen(fileName.toLocal8Bit().constData(), "wb");
  if (!outFile) {
    qmlInfo(this) << "Failed to open " << fileName;
    return false;
  }

  cinfo.err = jpeg_std_error((jpeg_error_mgr *)&jerr);
  jerr.error_exit = __error_exit;

  if (setjmp(jerr.setjmp_buffer)) {
    fclose(outFile);
    jpeg_destroy_compress(&cinfo);
    unlink(fileName.toLocal8Bit().constData());
    return false;
  }

  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outFile);

  cinfo.image_width = size.width();
  cinfo.image_height = size.height();
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  // TODO: configurable quality
  jpeg_set_quality (&cinfo, 100, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  JSAMPROW row_pointer;

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = (JSAMPROW) &data[cinfo.next_scanline*3*size.width()];
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);

  fclose(outFile);

  jpeg_destroy_compress(&cinfo);

  return true;
}

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
#include <tiffio.h>
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

class TiffWriter {
public:
  TiffWriter(int pages) : m_tif(0), m_page(0), m_pages(pages) {}
  ~TiffWriter() {
    close();
  }

  bool open(const QString& file) {
    if (m_tif) {
      return false;
    }

    m_tif = TIFFOpen(file.toLocal8Bit().constData(), "w");
    return m_tif != 0;
  }

  void close() {
    if (m_tif) {
      TIFFClose(m_tif);
      m_tif = 0;
    }
  }

  bool write(const guint8 *data, const QSize& size) {
    TIFFSetField(m_tif, TIFFTAG_IMAGEWIDTH, size.width());
    TIFFSetField(m_tif, TIFFTAG_IMAGELENGTH, size.height());
    TIFFSetField(m_tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(m_tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(m_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(m_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    // TODO: orientation
    TIFFSetField(m_tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(m_tif, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
    TIFFSetField(m_tif, TIFFTAG_PAGENUMBER, m_page++, m_pages);
    TIFFSetField(m_tif, TIFFTAG_COMPRESSION, COMPRESSION_CCITTRLE);

    for (int x = 0; x < size.height(); x++) {
      if (TIFFWriteScanline(m_tif, (void *)&data[x * size.width() * 3], x, 0) != 1) {
	qWarning() << "Failed to write scanline";
	return false;
      }

    }

    if (TIFFWriteDirectory(m_tif) != 1) {
      qWarning() << "Failed to write directory";
      return false;
    }

    return true;
  }

private:
  TIFF *m_tif;
  int m_page;
  int m_pages;
};

class YUVToRGB {
public:
  YUVToRGB(const QSize& size) : m_size(size) {
    m_rgb = new guint8[m_size.width() * m_size.height() * 3];
  }

  ~YUVToRGB() {
    delete [] m_rgb;
  }

  guint8 *convert(guint8 *data) {
    guint8 *y, *u, *v;

    y = data;
    u = y + m_size.width() * m_size.height();
    v = u + m_size.width()/2 * m_size.height()/2;
    if (libyuv::I420ToRGB24(y, m_size.width(),
			    u, m_size.width()/2,
			    v, m_size.width()/2,
			    m_rgb, m_size.width() * 3,
			    m_size.width(), m_size.height()) != 0) {
      return 0;
    }


    // TODO: find a better way
    // We need to convert the bgr we get from libyuv to rgb
    int size = m_size.width() * m_size.height() * 3;
    guint8 *s = m_rgb;
    guint8 a, b;
    for (int i = 0; i < size; i += 3) {
      a = s[0];
      b = s[2];
      s[0] = b;
      s[2] = a;
      s += 3;
    }

    return m_rgb;
  }

private:
  QSize m_size;
  guint8 *m_rgb;
};

PanoramaStitcher::PanoramaStitcher(std::vector<guint8 *>& frames, const QSize& size,
				   const QString& output,
				   bool keepFrames, QObject *parent) :
  QThread(parent),
  Stitcher(size.width(), size.height(), frames.size(), Stitcher::Wide),
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
  TiffWriter tif(m_keepFrames ? m_frames.size() + 1 : 1);
  YUVToRGB conv(m_size);

  int w, h;
  const unsigned char *im;

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

  if (!tif.open(m_output)) {
    emit error(Panorama::ErrorSave);
    goto out;
  }

  if (!tif.write(im, QSize(w, h))) {
    emit error(Panorama::ErrorSave);
    goto out;
  }

  if (m_keepFrames) {
    for (int x = 0; x < m_frames.size(); x++) {

      // YUV to RGB conversion
      guint8 *data = conv.convert(m_frames[x]);

      if (!data) {
	emit error(Panorama::ErrorIntermediatesConvert);
	tif.close();
	goto out;
      }

      if (!tif.write(data, m_size)) {
	emit error(Panorama::ErrorIntermediatesSave);
	tif.close();
	goto out;
      }
    }
  }

  tif.close();

out:
  Panorama::clear(m_frames);
  emit done();
}

int PanoramaStitcher::progress() {
  QMutexLocker l(&m_progressLock);
  return m_alignProgress + Stitcher::progress();
}

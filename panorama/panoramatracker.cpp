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

#include "panoramatracker.h"
#include "panorama.h"
#include "libyuv.h"
#include "qtcamgstsample.h"
#include "panoramainput.h"
#include <QDebug>
#include <QScopedPointer>
#include <QScopedArrayPointer>
#include <algorithm>

PanoramaTracker::PanoramaTracker(PanoramaInput *input, QObject *parent) :
  QThread(parent),
  Tracker(MAX_TRACKER_FRAMES),
  m_width(-1),
  m_height(-1),
  m_running(true),
  m_input(input) {

  m_scaled.reserve(MAX_TRACKER_FRAMES);
  m_frames.reserve(MAX_TRACKER_FRAMES);
}

PanoramaTracker::~PanoramaTracker() {
  m_lock.lock();

  Panorama::clear(m_scaled);
  Panorama::clear(m_frames);

  m_lock.unlock();
}

bool PanoramaTracker::isRunning() {
  QMutexLocker l(&m_lock);
  return m_running;
}

int PanoramaTracker::frameCount() {
  QMutexLocker l(&m_lock);
  return m_scaled.size();
}

void PanoramaTracker::stop() {
  m_lock.lock();
  m_running = false;
  m_lock.unlock();
  wait();
}

void PanoramaTracker::run() {
  while (isRunning() && m_scaled.size() <= MAX_TRACKER_FRAMES) {
    QScopedPointer<QtCamGstSample> sample(m_input->sample());

    if (!sample) {
      continue;
    }

    if (!Tracker::isInitialized()) {
      QSize size = QSize(sample->width(), sample->height());
      int m_width = size.width() > 720 ? size.width() / 8 : size.width() / 4;
      int m_height = size.width() > 720 ? size.height() / 8 : size.height() / 4;
      m_inputSize = size;

      // TODO: This should be 5.0 but we fail to stitch sometimes if we set it to 5
      if (!Tracker::initialize(m_width, m_height, 2.0f)) {
	emit error(Panorama::ErrorTrackerInit);
	return;
      }
    }

    // Now we can process the sample:
    const guint8 *src = sample->data();

    QScopedArrayPointer<guint8>
      dst(new guint8[m_inputSize.width() * m_inputSize.height() * 3 / 2]);
    enum libyuv::FourCC fmt;

    switch (sample->format()) {
    case GST_VIDEO_FORMAT_UYVY:
      fmt = libyuv::FOURCC_UYVY;
      break;
    default:
      qCritical() << "Unsupported color format";
      emit error(Panorama::ErrorTrackerFormat);
      return;
    }

    guint8 *y = dst.data(),
      *u = y + m_inputSize.width() * m_inputSize.height(),
      *v = u + m_inputSize.width()/2 * m_inputSize.height()/2;

    if (ConvertToI420(src, sample->size(),
		      y, m_inputSize.width(),
		      u, m_inputSize.width() / 2,
		      v, m_inputSize.width() / 2,
		      0, 0,
		      m_inputSize.width(), m_inputSize.height(),
		      m_inputSize.width(), m_inputSize.height(),
		      libyuv::kRotate0, fmt) != 0) {
      emit error(Panorama::ErrorTrackerConvert);
      return;
    }

    QScopedArrayPointer<guint8> scaled(new guint8[m_width * m_height * 3 / 2]);
    guint8 *ys = scaled.data(),
      *us = ys + m_width * m_height,
      *vs = us + m_width/2 * m_height/2;

    // Now scale:
    // No need for error checking because the function always returns 0
    libyuv::I420Scale(y, m_inputSize.width(),
		      u, m_inputSize.width()/2,
		      v, m_inputSize.width()/2,
		      m_inputSize.width(), m_inputSize.height(),
		      ys, m_width,
		      us, m_width/2,
		      vs, m_width/2,
		      m_width, m_height,
		      libyuv::kFilterBilinear);

    int err = addFrame(scaled.data());

    if (err >= 0) {
      m_scaled.push_back(scaled.take());
      m_frames.push_back(dst.take());
      emit frameCountChanged();
    }
  }
}

void PanoramaTracker::releaseFrames(std::vector<guint8 *>& frames) {
  QMutexLocker l(&m_lock);
  std::swap(m_frames, frames);
}

QSize PanoramaTracker::size() {
  QMutexLocker l(&m_lock);
  return m_inputSize;
}

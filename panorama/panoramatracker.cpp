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
#include <QDebug>

PanoramaTracker::PanoramaTracker(QObject *parent) :
  QThread(parent),
  Tracker(MAX_TRACKER_FRAMES),
  m_width(0),
  m_height(0),
  m_running(true) {

  m_input.reserve(5);
  m_scaled.reserve(MAX_TRACKER_FRAMES);
  m_frames = new std::vector<uint8_t *>();
  m_frames->reserve(MAX_TRACKER_FRAMES);
}

PanoramaTracker::~PanoramaTracker() {
  m_lock.lock();

  Panorama::clear(m_input);
  Panorama::clear(m_scaled);

  if (m_frames) {
    Panorama::clear(*m_frames);
    delete m_frames;
    m_frames = 0;
  }

  m_lock.unlock();
}

int PanoramaTracker::frameCount() {
  QMutexLocker l(&m_lock);
  return m_scaled.size();
}

bool PanoramaTracker::handleData(uint8_t *data, const QSize& size) {
  if (!m_lock.tryLock()) {
    return false;
  }

  if (!Tracker::isInitialized()) {
    int m_width = size.width() > 720 ? size.width() / 8 : size.width() / 4;
    int m_height = size.width() > 720 ? size.height() / 8 : size.height() / 4;
    m_inputSize = size;

    // TODO: error
    Tracker::initialize(m_width, m_height);
  }

  if (m_frames->size() >= MAX_TRACKER_FRAMES) {
    m_lock.unlock();
    return false;
  }

  m_input.push_back(data);
  m_cond.wakeAll();

  m_lock.unlock();

  return true;
}

void PanoramaTracker::stop() {
  m_lock.lock();
  m_running = false;
  m_cond.wakeAll();
  m_lock.unlock();
  wait();
}

void PanoramaTracker::run() {
  m_lock.lock();

  while (m_running && m_scaled.size() <= MAX_TRACKER_FRAMES) {
    if (m_input.size() == 0) {
      m_cond.wait(&m_lock);
    }

    if (!m_running) {
      break;
    }

    if (!m_frames) {
      break;
    }

    uint8_t *input = m_input[m_input.size() - 1];
    m_input.pop_back();

    Panorama::clear(m_input);

    uint8_t *scaled = new uint8_t[m_width * m_height * 3 / 2];

    uint8_t *y = input,
      *u = y + m_inputSize.width() * m_inputSize.height(),
      *v = u + m_inputSize.width()/2 * m_inputSize.height()/2;

    uint8_t *ys = scaled,
      *us = ys + m_width * m_height,
      *vs = us + m_width/2 * m_height/2;

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

    // TODO:
    int err = addFrame(scaled);

    if (err >= 0) {
      m_scaled.push_back(scaled);
      m_frames->push_back(input);
      emit frameCountChanged();
    } else {
      delete [] scaled;
      delete [] input;
    }
  }

  Panorama::clear(m_input);

  m_lock.unlock();
}

std::vector<uint8_t *> *PanoramaTracker::releaseFrames() {
  QMutexLocker l(&m_lock);
  std::vector<uint8_t *> *frames = m_frames;
  m_frames = 0;
  return frames;
}

QSize PanoramaTracker::size() {
  QMutexLocker l(&m_lock);
  return m_inputSize;
}

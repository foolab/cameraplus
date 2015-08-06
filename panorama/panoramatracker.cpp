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
  Tracker(TRACKER_WIDTH, TRACKER_HEIGHT, MAX_TRACKER_FRAMES),
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

bool PanoramaTracker::handleData(uint8_t *data) {
  if (!m_lock.tryLock()) {
    return false;
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

    uint8_t *scaled = new uint8_t[TRACKER_WIDTH * TRACKER_HEIGHT * 3 / 2];

    int err = libyuv::I420Scale(FRAME_Y(input), FRAME_WIDTH,
				FRAME_U(input), FRAME_WIDTH/2,
				FRAME_V(input), FRAME_WIDTH/2,
				FRAME_WIDTH, FRAME_HEIGHT,
				TRACKER_Y(scaled), TRACKER_WIDTH,
				TRACKER_U(scaled), TRACKER_WIDTH/2,
				TRACKER_V(scaled), TRACKER_WIDTH/2,
				TRACKER_WIDTH, TRACKER_HEIGHT,
				libyuv::kFilterBilinear);
    if (err != 0) {
      // TODO: error
    }

    err = addFrame(scaled);

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

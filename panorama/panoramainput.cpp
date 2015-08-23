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

#include "panoramainput.h"
#include "qtcamgstsample.h"

PanoramaInput::PanoramaInput(QObject *parent) :
  QObject(parent),
  m_sample(0),
  m_running(false) {

}

PanoramaInput::~PanoramaInput() {
  stop();
}

void PanoramaInput::start() {
  QMutexLocker l(&m_lock);

  m_running = true;
}

void PanoramaInput::stop() {
  QMutexLocker l(&m_lock);

  if (m_sample) {
    delete m_sample;
    m_sample = 0;
  }

  m_running = false;

  m_cond.wakeOne();
}

void PanoramaInput::handleSample(const QtCamGstSample *sample) {
  QMutexLocker l(&m_lock);

  if (m_sample) {
    delete m_sample;
  }

  if (!m_running) {
    return;
  }

  m_sample = new QtCamGstSample(*sample);

  m_cond.wakeOne();
}

QtCamGstSample *PanoramaInput::sample() {
  QMutexLocker l(&m_lock);
  if (!m_running) {
    return 0;
  }

  if (m_sample) {
    QtCamGstSample *sample = m_sample;
    m_sample = 0;
    return sample;
  }

  m_cond.wait(&m_lock);

  if (!m_running) {
    return 0;
  }

  if (m_sample) {
    // We must have a sample
    QtCamGstSample *sample = m_sample;
    m_sample = 0;
    return sample;
  }

  return 0;
}

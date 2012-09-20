/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#include "videomode.h"
#include "qtcamvideomode.h"
#include "qtcamdevice.h"
#include "camera.h"

VideoMode::VideoMode(QObject *parent) :
  Mode(parent),
  m_video(0) {

}

VideoMode::~VideoMode() {
  m_video = 0;
}


bool VideoMode::startRecording(const QString& fileName) {
  return m_video ? m_video->startRecording(fileName) : false;
}

void VideoMode::stopRecording() {
  if (m_video) {
    m_video->stopRecording();
  }
}

void VideoMode::preChangeMode() {
  if (m_video) {
    QObject::disconnect(m_video, SIGNAL(recordingStateChanged()),
			this, SIGNAL(recordingStateChanged()));
  }

  m_video = 0;
}

void VideoMode::postChangeMode() {
  m_video = m_cam->device()->videoMode();

  if (m_video) {
    QObject::connect(m_video, SIGNAL(recordingStateChanged()),
			this, SIGNAL(recordingStateChanged()));
  }
}

bool VideoMode::isRecording() {
  return m_video ? m_video->isRecording() : false;
}

void VideoMode::changeMode() {
  m_mode = m_cam->device()->videoMode();
}

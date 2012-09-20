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

#include "videotorch.h"
#include "qtcamvideotorch.h"
#include "camera.h"
#include "qtcamdevice.h"
#include "qtcamvideomode.h"

VideoTorch::VideoTorch(QObject *parent) :
  QObject(parent), m_cam(0), m_torch(0) {

}

VideoTorch::~VideoTorch() {
  m_cam = 0;
}

Camera *VideoTorch::camera() {
  return m_cam;
}

void VideoTorch::setCamera(Camera *camera) {
  if (camera == m_cam) {
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  m_cam = camera;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  emit cameraChanged();

  deviceChanged();
}

bool VideoTorch::isOn() const {
  return m_torch ? m_torch->isOn() : false;
}

void VideoTorch::setOn(bool on) {
  if (m_torch) {
    m_torch->setOn(on);
  }
}

void VideoTorch::deviceChanged() {
  if (m_torch) {
    delete m_torch; m_torch = 0;
  }

  if (m_cam->device()) {
    m_torch = new QtCamVideoTorch(m_cam->device(), this);
    QObject::connect(m_torch, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));

    emit stateChanged();
  }
}

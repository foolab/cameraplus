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

#include "capability.h"
#include "camera.h"

Capability::Capability(QObject *parent) :
  QObject(parent),
  m_cam(0) {

}

Capability::~Capability() {

}

Camera *Capability::camera() {
  return m_cam;
}

void Capability::setCamera(Camera *cam) {
  if (cam == m_cam) {
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SIGNAL(isReadyChanged()));
  }

  m_cam = cam;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SIGNAL(isReadyChanged()));
  }

  emit cameraChanged();

  deviceChanged();

  emit isReadyChanged();
}

bool Capability::isReady() const {
  return m_cam && m_cam->device();
}

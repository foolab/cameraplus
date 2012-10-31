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

#include "autofocus.h"
#include "camera.h"

AutoFocus::AutoFocus(QObject *parent) :
  Capability(parent),
  m_af(0) {

}
AutoFocus::~AutoFocus() {
  if (m_af) {
    delete m_af; m_af = 0;
  }
}

void AutoFocus::deviceChanged() {
  if (m_af) {
    delete m_af; m_af = 0;
  }

  if (m_cam->device()) {
    m_af = new QtCamAutoFocus(m_cam->device(), this);
    QObject::connect(m_af, SIGNAL(statusChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_af, SIGNAL(cafStatusChanged()), this, SIGNAL(cafValueChanged()));
  }

  emit valueChanged();
  emit cafValueChanged();
}

AutoFocus::Status AutoFocus::status() {
  return m_af ? (AutoFocus::Status)m_af->status() : AutoFocus::None;
}

AutoFocus::Status AutoFocus::cafStatus() {
  return m_af ? (AutoFocus::Status)m_af->cafStatus() : AutoFocus::None;
}

bool AutoFocus::startAutoFocus() {
  if (m_af) {
    return m_af->startAutoFocus();
  }

  return false;
}

bool AutoFocus::stopAutoFocus() {
  if (m_af) {
    return m_af->stopAutoFocus();
  }

  return false;
}

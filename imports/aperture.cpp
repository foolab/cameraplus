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

#include "aperture.h"
#include "camera.h"
#include "qtcamaperture.h"

Aperture::Aperture(QObject *parent) :
  Capability(parent),
  m_aperture(0) {

}

Aperture::~Aperture() {
  if (m_aperture) {
    delete m_aperture; m_aperture = 0;
  }
}

void Aperture::deviceChanged() {
  if (m_aperture) {
    delete m_aperture; m_aperture = 0;
  }

  if (m_cam->device()) {
    m_aperture = new QtCamAperture(m_cam->device(), this);
    QObject::connect(m_aperture, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_aperture, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
    QObject::connect(m_aperture, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
  }

  emit valueChanged();
  emit minimumChanged();
  emit maximunmChanged();
}

unsigned int Aperture::value() {
  return m_aperture ? m_aperture->value() : 0;
}

void Aperture::setValue(unsigned int val) {
  if (m_aperture) {
    m_aperture->setValue(val);
  }
}

unsigned int Aperture::minimum() {
  return m_aperture ? m_aperture->minimumValue() : 0;
}

unsigned int Aperture::maximum() {
  return m_aperture ? m_aperture->maximumValue() : 0;
}

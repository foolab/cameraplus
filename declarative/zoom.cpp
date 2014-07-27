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

#include "zoom.h"
#include "qtcamzoom.h"

Zoom::Zoom(QtCamDevice *dev, QObject *parent) :
  Capability(parent),
  m_zoom(new QtCamZoom(dev, this)) {

  setCapability(m_zoom);

  QObject::connect(m_zoom, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  QObject::connect(m_zoom, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
  QObject::connect(m_zoom, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
}

Zoom::~Zoom() {
  if (m_zoom) {
    delete m_zoom;
    m_zoom = 0;
  }
}

qreal Zoom::value() {
  return m_zoom ? m_zoom->value() : 1.0;
}

void Zoom::setValue(qreal val) {
  if (m_zoom) {
    m_zoom->setValue(val);
  }
}

qreal Zoom::minimum() {
  return m_zoom ? m_zoom->minimumValue() : 1.0;
}

qreal Zoom::maximum() {
  return m_zoom ? m_zoom->maximumValue() : 1.0;
}

void Zoom::prepareForDeviceChange() {
  if (m_zoom) {
    delete m_zoom;
    m_zoom = 0;
  }
}

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

#include "colortone.h"
#include "camera.h"

ColorTone::ColorTone(QObject *parent) :
  Capability(parent),
  m_color(0) {

}

ColorTone::~ColorTone() {
  if (m_color) {
    delete m_color; m_color = 0;
  }
}

void ColorTone::deviceChanged() {
  if (m_color) {
    delete m_color; m_color = 0;
  }

  if (m_cam->device()) {
    m_color = new QtCamColorTone(m_cam->device(), this);
    QObject::connect(m_color, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  }

  emit valueChanged();
}

ColorTone::ColorToneMode ColorTone::value() {
  return m_color ? (ColorToneMode)m_color->value() : ColorTone::Normal;
}

void ColorTone::setValue(const ColorTone::ColorToneMode& mode) {
  if (m_color) {
    m_color->setValue((QtCamColorTone::ColorToneMode)mode);
  }
}

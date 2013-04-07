/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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
#include "qtcamaperture.h"

Aperture::Aperture(QtCamDevice *dev, QObject *parent) :
  Capability(parent),
  m_aperture(new QtCamAperture(dev, this)) {

  setCapability(m_aperture);

  QObject::connect(m_aperture, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  QObject::connect(m_aperture, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
  QObject::connect(m_aperture, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
}

Aperture::~Aperture() {
  delete m_aperture; m_aperture = 0;
}

unsigned int Aperture::value() {
  return m_aperture->value();
}

void Aperture::setValue(unsigned int val) {
  m_aperture->setValue(val);
}

unsigned int Aperture::minimum() {
  return m_aperture->minimumValue();
}

unsigned int Aperture::maximum() {
  return m_aperture->maximumValue();
}

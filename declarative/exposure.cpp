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

#include "exposure.h"
#include "camera.h"
#include "qtcamexposure.h"

Exposure::Exposure(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  m_exposure(new QtCamExposure(dev, this)) {

  QObject::connect(m_exposure, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  QObject::connect(m_exposure, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
  QObject::connect(m_exposure, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
}

Exposure::~Exposure() {
  delete m_exposure; m_exposure = 0;
}

unsigned int Exposure::value() {
  return m_exposure->value();
}

void Exposure::setValue(unsigned int val) {
  m_exposure->setValue(val);
}

unsigned int Exposure::minimum() {
  return m_exposure->minimumValue();
}

unsigned int Exposure::maximum() {
  return m_exposure->maximumValue();
}

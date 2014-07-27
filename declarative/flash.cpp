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

#include "flash.h"

Flash::Flash(QtCamDevice *dev, QObject *parent) :
  Capability(parent),
  m_flash(new QtCamFlash(dev, this)) {

  setCapability(m_flash);

  QObject::connect(m_flash, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  QObject::connect(m_flash, SIGNAL(flashReadyChanged()), this, SIGNAL(readyChanged()));
}

Flash::~Flash() {
  if (m_flash) {
    delete m_flash;
    m_flash = 0;
  }
}

Flash::FlashMode Flash::value() {
  return m_flash ? (FlashMode)m_flash->value() : Flash::Auto;
}

void Flash::setValue(const Flash::FlashMode& mode) {
  if (m_flash) {
    m_flash->setValue((QtCamFlash::FlashMode)mode);
  }
}

bool Flash::isReady() const {
  return m_flash ? m_flash->isReady() : true;
}

void Flash::prepareForDeviceChange() {
  if (m_flash) {
    delete m_flash;
    m_flash = 0;
  }
}

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

AutoFocus::AutoFocus(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  m_af(new QtCamAutoFocus(dev, this)) {

  QObject::connect(m_af, SIGNAL(statusChanged()), this, SIGNAL(valueChanged()));
  QObject::connect(m_af, SIGNAL(cafStatusChanged()), this, SIGNAL(cafValueChanged()));
}

AutoFocus::~AutoFocus() {
  delete m_af; m_af = 0;
}

AutoFocus::Status AutoFocus::status() {
  return (AutoFocus::Status)m_af->status();
}

AutoFocus::Status AutoFocus::cafStatus() {
  return (AutoFocus::Status)m_af->cafStatus();
}

bool AutoFocus::startAutoFocus() {
  return m_af->startAutoFocus();
}

bool AutoFocus::stopAutoFocus() {
  return m_af->stopAutoFocus();
}

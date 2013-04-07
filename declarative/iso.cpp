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

#include "iso.h"
#include "qtcamiso.h"

Iso::Iso(QtCamDevice *dev, QObject *parent) :
  Capability(parent),
  m_iso(new QtCamIso(dev, this)) {

  setCapability(m_iso);

  QObject::connect(m_iso, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  QObject::connect(m_iso, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
  QObject::connect(m_iso, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
}

Iso::~Iso() {
  delete m_iso; m_iso = 0;
}

unsigned int Iso::value() {
  return m_iso->value();
}

void Iso::setValue(unsigned int val) {
  m_iso->setValue(val);
}

unsigned int Iso::minimum() {
  return m_iso->minimumValue();
}

unsigned int Iso::maximum() {
  return m_iso->maximumValue();
}

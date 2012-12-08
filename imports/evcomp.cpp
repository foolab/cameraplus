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

#include "evcomp.h"
#include "qtcamevcomp.h"

EvComp::EvComp(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  m_evComp(new QtCamEvComp(dev, this)) {

  QObject::connect(m_evComp, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  QObject::connect(m_evComp, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
  QObject::connect(m_evComp, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
}

EvComp::~EvComp() {
  delete m_evComp; m_evComp = 0;
}

qreal EvComp::value() {
  return m_evComp->value();
}

void EvComp::setValue(qreal val) {
  m_evComp->setValue(val);
}

qreal EvComp::minimum() {
  return m_evComp->minimumValue();
}

qreal EvComp::maximum() {
  return m_evComp->maximumValue();
}

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
#include "camera.h"
#include "qtcamevcomp.h"

EvComp::EvComp(QObject *parent) :
  Capability(parent),
  m_evComp(0) {

}

EvComp::~EvComp() {
  if (m_evComp) {
    delete m_evComp; m_evComp = 0;
  }
}

void EvComp::deviceChanged() {
  if (m_evComp) {
    delete m_evComp; m_evComp = 0;
  }

  if (m_cam->device()) {
    m_evComp = new QtCamEvComp(m_cam->device(), this);
    QObject::connect(m_evComp, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_evComp, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
    QObject::connect(m_evComp, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
  }

  emit valueChanged();
  emit minimumChanged();
  emit maximunmChanged();
}

qreal EvComp::value() {
  return m_evComp ? m_evComp->value() : 0.0;
}

void EvComp::setValue(qreal val) {
  if (m_evComp) {
    m_evComp->setValue(val);
  }
}

qreal EvComp::minimum() {
  return m_evComp ? m_evComp->minimumValue() : 0.0;
}

qreal EvComp::maximum() {
  return m_evComp ? m_evComp->maximumValue() : 0.0;
}

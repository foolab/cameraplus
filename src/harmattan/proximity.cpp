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

#include "proximity.h"
#include <QDebug>

Proximity::Proximity(QObject *parent) :
  QObject(parent),
  m_proximity(new MeeGo::QmProximity(this)),
  m_close(false) {

  QObject::connect(m_proximity, SIGNAL(ProximityChanged(const MeeGo::QmProximityReading)),
		   this, SLOT(proximityChanged(const MeeGo::QmProximityReading)));

  if (m_proximity->requestSession(MeeGo::QmSensor::SessionTypeListen)
      == MeeGo::QmSensor::SessionTypeNone) {
    qDebug() << "Failed to get listen session:" << m_proximity->lastError();
  }
}

Proximity::~Proximity() {
  m_proximity->stop();
}

bool Proximity::isActive() const {
  return m_proximity->isRunning();
}

void Proximity::setActive(bool active) {
  if (active == isActive()) {
    return;
  }

  if (active) {
    m_proximity->start();
  }
  else {
    m_proximity->stop();
  }

  emit activeChanged();
}

bool Proximity::isClose() const {
  return m_close;
}

void Proximity::proximityChanged(const MeeGo::QmProximityReading& reading) {
  bool close = reading.value ? true : false;

  if (close != m_close) {
    m_close = close;
    emit closeChanged();
  }
}

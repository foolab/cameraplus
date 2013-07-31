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

#include "compass.h"
#include <qmcompass.h>
#include <QDebug>

Compass::Compass(QObject *parent) :
  QObject(parent),
  m_compass(new MeeGo::QmCompass(this)),
  m_degree(-1),
  m_valid(false) {

  m_compass->setUseDeclination(true);

  QObject::connect(m_compass, SIGNAL(dataAvailable(const MeeGo::QmCompassReading&)),
		   this, SLOT(dataAvailable(const MeeGo::QmCompassReading&)));

  if (m_compass->requestSession(MeeGo::QmSensor::SessionTypeListen)
      == MeeGo::QmSensor::SessionTypeNone) {
    qDebug() << "Failed to get listen session:" << m_compass->lastError();
  }
}

Compass::~Compass() {
  m_compass->stop();
}

bool Compass::isActive() const {
  return m_compass->isRunning();
}

void Compass::setActive(bool active) {
  if (active == isActive()) {
    return;
  }

  if (active) {
    m_compass->start();
  }
  else {
    m_compass->stop();

    m_valid = false;
    emit directionValidChanged();
  }

  emit activeChanged();
}

int Compass::direction() const {
  return m_degree;
}

bool Compass::isDirectionValid() const {
  return m_valid;
}

void Compass::dataAvailable(const MeeGo::QmCompassReading& value) {
  bool degreeChanged = (m_degree != value.degrees);
  bool validityChanged = m_valid != (value.level >= 2);

  m_degree = value.degrees;
  m_valid = (value.level >= 2);

  if (validityChanged) {
    emit directionValidChanged();
  }

  if (degreeChanged) {
    emit directionChanged();
  }
}

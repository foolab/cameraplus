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

#include "orientation.h"
#include <QOrientationSensor>
#include <QDebug>

Orientation::Orientation(QObject *parent) :
  QObject(parent),
  m_sensor(new QOrientationSensor(this)),
  m_direction(Unknown),
  m_orientationAngle(-1) {

  QObject::connect(m_sensor, SIGNAL(readingChanged()),
		   this, SLOT(readingChanged()));
}

Orientation::~Orientation() {
  delete m_sensor;
  m_sensor = 0;
}

bool Orientation::isActive() const {
  return m_sensor->isActive();
}

void Orientation::setActive(bool active) {
  if (active == isActive()) {
    return;
  }

  if (active) {
    m_sensor->start();
    readingChanged();
  }
  else {
    m_sensor->stop();
    m_direction = Unknown;

    emit orientationChanged();
  }

  emit activeChanged();
}

Orientation::OrientationDirection Orientation::orientation() const {
  return m_direction;
}

void Orientation::readingChanged() {
  OrientationDirection direction = Unknown;

  QOrientationReading *value = m_sensor->reading();

  switch (value->orientation()) {
  case QOrientationReading::LeftUp:
    setOrientationAngle(90);
    direction = InvertedLandscape;
    break;

  case QOrientationReading::RightUp:
    setOrientationAngle(270);
    direction = Landscape;
    break;

  case QOrientationReading::TopUp:
    setOrientationAngle(0);
    direction = Portrait;
    break;

  case QOrientationReading::TopDown:
    setOrientationAngle(180);
    direction = InvertedPortrait;
    break;

  default:
    setOrientationAngle(-1);
    direction = Unknown;
    break;
  }

  if (direction != m_direction) {
    m_direction = direction;

    emit orientationChanged();
  }
}

void Orientation::setOrientationAngle(int angle) {
  if (m_orientationAngle != angle) {
    m_orientationAngle = angle;
    emit orientationAngleChanged();
  }
}

int Orientation::orientationAngle() const {
  return m_orientationAngle;
}

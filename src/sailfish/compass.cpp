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

#include "compass.h"
#include <QCompass>
#include <QDebug>

Compass::Compass(QObject *parent) :
  QObject(parent),
  m_compass(new QCompass(this)),
  m_degree(-1),
  m_valid(false) {

  QObject::connect(m_compass, SIGNAL(readingChanged()),
		   this, SLOT(readingChanged()));
}

Compass::~Compass() {
  delete m_compass;
  m_compass = 0;
}

bool Compass::isActive() const {
  return m_compass->isActive();
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

void Compass::readingChanged() {
  QCompassReading *value = m_compass->reading();

  bool degreeChanged = (m_degree != value->azimuth());
  bool validityChanged = m_valid != true;

  m_degree = value->azimuth();
  m_valid = true;

  if (validityChanged) {
    emit directionValidChanged();
  }

  if (degreeChanged) {
    emit directionChanged();
  }
}

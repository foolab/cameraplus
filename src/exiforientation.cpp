// -*- c++ -*-

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

#include "exiforientation.h"

ExifOrientation::ExifOrientation() :
  m_direction(Orientation::Unknown),
  m_sensorAngle(-1),
  m_isFront(false),
  m_angle(0) {

}

ExifOrientation::~ExifOrientation() {

}

Orientation::OrientationDirection ExifOrientation::orientation() const {
  return m_direction;
}

void ExifOrientation::setOrientation(const Orientation::OrientationDirection& orientation) {
  if (m_direction != orientation) {
    m_direction = orientation;

    emit orientationChanged();

    calculate();
  }
}

int ExifOrientation::sensorOrientation() const {
  return m_sensorAngle;
}

void ExifOrientation::setSensorOrientation(int orientation) {
  if (m_sensorAngle != orientation) {
    m_sensorAngle = orientation;

    emit sensorOrientationChanged();

    calculate();
  }
}

bool ExifOrientation::isFrontCamera() const {
  return m_isFront;
}

void ExifOrientation::setFrontCamera(bool front) {
  if (m_isFront != front) {
    m_isFront = front;

    emit frontCameraChanged();

    calculate();
  }
}

void ExifOrientation::setOrientationAngle(int angle) {
  if (m_angle != angle) {
    m_angle = angle;
    emit orientationAngleChanged();
  }
}

int ExifOrientation::orientationAngle() const {
  return m_angle;
}

// based on this:
// http://developer.android.com/reference/android/hardware/Camera.Parameters.html#setRotation(int)
void ExifOrientation::calculate() {
  // Nothing is known:
  if (m_direction == Orientation::Unknown) {
    setOrientationAngle(-1);
    return;
  }

  // If the sensor orientation is unknown then we depend on the
  // device orientation only. This happens on Harmattan:
  if (m_sensorAngle == -1) {
    setOrientationAngle(m_direction * 90);
    return;
  }

  // Now Sailfish OS:
  // TODO: We will assume portrait is the natural orientation for now:
  // http://developer.android.com/reference/android/view/OrientationEventListener.html#onOrientationChanged(int)
  // Convert to android compatible orientation
  int angle = (((m_direction - 1) * 90) + 360) % 360;

  if (m_isFront) {
    setOrientationAngle((m_sensorAngle - angle + 360) % 360);
  } else {
    setOrientationAngle((m_sensorAngle + angle) % 360);
  }
}

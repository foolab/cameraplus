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

#include "position.h"

Position::Position(QObject *parent) :
  QObject(parent),
  m_deviceOrientationAngle(0),
  m_sensorOrientationAngle(0),
  m_isFront(false),
  m_applicationOrientationAngle(0),
  m_naturalOrientation(Portrait),
  m_viewfinderFlipped(false) {

  update();
}

Position::~Position() {

}

int Position::deviceOrientationAngle() const {
  return m_deviceOrientationAngle;
}

void Position::setDeviceOrientationAngle(int angle) {
  if (m_deviceOrientationAngle != angle) {
    m_deviceOrientationAngle = angle;
    emit deviceOrientationAngleChanged();
    update();
  }
}

int Position::sensorOrientationAngle() const {
  return m_sensorOrientationAngle;
}

void Position::setSensorOrientationAngle(int angle) {
  if (m_sensorOrientationAngle != angle) {
    m_sensorOrientationAngle = angle;
    emit sensorOrientationAngleChanged();
    update();
  }
}

bool Position::isFrontCamera() const {
  return m_isFront;
}

void Position::setFrontCamera(bool isFront) {
  if (m_isFront != isFront) {
    m_isFront = isFront;
    emit isFrontCameraChanged();
    update();
  }
}

int Position::applicationOrientationAngle() const {
  return m_applicationOrientationAngle;
}

void Position::setApplicationOrientationAngle(int angle) {
  if (m_applicationOrientationAngle != angle) {
    m_applicationOrientationAngle = angle;
    emit applicationOrientationAngleChanged();
    update();
  }
}

Position::NaturalOrientation Position::naturalOrientation() const {
  return m_naturalOrientation;
}

void Position::setNaturalOrientation(Position::NaturalOrientation orientation) {
  if (m_naturalOrientation != orientation) {
    m_naturalOrientation = orientation;
    emit naturalOrientationChanged();
    update();
  }
}

int Position::viewfinderRotationAngle() const {
  return m_viewfinderRotationAngle;
}

void Position::setViewfinderRotationAngle(int angle) {
  if (m_viewfinderRotationAngle != angle) {
    m_viewfinderRotationAngle = angle;
    emit viewfinderRotationAngleChanged();
  }
}

bool Position::isViewfinderFlipped() const {
  return m_viewfinderFlipped;
}

void Position::setViewfinderFlipped(bool flipped) {
  if (m_viewfinderFlipped != flipped) {
    m_viewfinderFlipped = flipped;
    emit isViewfinderFlippedChanged();
  }
}

int Position::exifRotationAngle() const {
  return m_exifRotationAngle;
}

void Position::setExifRotationAngle(int angle) {
  if (m_exifRotationAngle != angle) {
    m_exifRotationAngle = angle;
    emit exifRotationAngleChanged();
  }
}

bool Position::isExifFlipped() const {
  return m_exifFlipped;
}

void Position::setExifFlipped(bool flipped) {
  if (m_exifFlipped != flipped) {
    m_exifFlipped = flipped;
    emit isExifFlippedChanged();
  }
}

bool Position::isPortrait() const {
  return m_isPortrait;
}

void Position::setPortrait(bool portrait) {
  if (m_isPortrait != portrait) {
    m_isPortrait = portrait;
    emit isPortraitChanged();
  }
}

void Position::update() {
  // If we are using front camera then we are flipped:
  setExifFlipped(m_isFront);

  // Now the device orientation:
  if (m_deviceOrientationAngle != -1) {
    if (m_naturalOrientation == Position::Portrait) {
      setPortrait(m_deviceOrientationAngle == 0 || m_deviceOrientationAngle == 180);
    } else {
      setPortrait(m_deviceOrientationAngle == 90 || m_deviceOrientationAngle == 270);
    }
  } else {
    setPortrait(m_naturalOrientation == Position::Portrait);
  }

  // viewfinder flipped:
  setViewfinderFlipped(m_isFront);

  // viewfinder rotation angle:
  // This is based on android setCameraDisplayOrientation()
  if (m_isFront) { // front-facing
    int result = (m_sensorOrientationAngle + m_applicationOrientationAngle) % 360;
    result = (360 - result) % 360;  // compensate the mirror
    setViewfinderRotationAngle(result);
  } else {  // back-facing
    int result = (m_sensorOrientationAngle - m_applicationOrientationAngle + 360) % 360;
    setViewfinderRotationAngle(result);
  }

  // exif rotation angle:
  if (m_deviceOrientationAngle == -1) {
    // We will set to -1 so we stop adding exif.
    setExifRotationAngle(-1);
  } else {
    // based on this:
    // http://developer.android.com/reference/android/hardware/Camera.Parameters.html#setRotation(int)
    // This is simply madness: http://www.impulseadventure.com/photo/exif-orientation.html
    // NOTE: our Orientation implementation behaves as android does
    if (m_isFront) {
      setExifRotationAngle((m_sensorOrientationAngle - m_deviceOrientationAngle + 360) % 360);
    } else {
      setExifRotationAngle((m_sensorOrientationAngle + m_deviceOrientationAngle) % 360);
    }
  }
}

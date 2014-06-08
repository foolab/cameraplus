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

#ifndef EXIF_ORIENTATION_H
#define EXIF_ORIENTATION_H

#include <QObject>
#include "orientation.h"

class ExifOrientation : public QObject {
  Q_OBJECT

  Q_PROPERTY(Orientation::OrientationDirection orientation READ orientation WRITE setOrientation NOTIFY orientationChanged);
  Q_PROPERTY(int sensorOrientation READ sensorOrientation WRITE setSensorOrientation NOTIFY sensorOrientationChanged);
  Q_PROPERTY(bool frontCamera READ isFrontCamera WRITE setFrontCamera NOTIFY frontCameraChanged);
  Q_PROPERTY(int orientationAngle READ orientationAngle NOTIFY orientationAngleChanged);

public:
  ExifOrientation(QObject *parent = 0);
  ~ExifOrientation();

  Orientation::OrientationDirection orientation() const;
  void setOrientation(const Orientation::OrientationDirection& orientation);

  int sensorOrientation() const;
  void setSensorOrientation(int orientation);

  bool isFrontCamera() const;
  void setFrontCamera(bool front);

  int orientationAngle() const;
  void setOrientationAngle(int angle);

signals:
  void orientationChanged();
  void sensorOrientationChanged();
  void frontCameraChanged();
  void orientationAngleChanged();

private:
  void calculate();

  Orientation::OrientationDirection m_direction;
  int m_sensorAngle;
  bool m_isFront;
  int m_angle;
};

#endif /* EXIF_ORIENTATION_H */

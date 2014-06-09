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

#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <QObject>

class QOrientationSensor;

class Orientation : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(OrientationDirection orientation READ orientation NOTIFY orientationChanged);
  Q_PROPERTY(int orientationAngle READ orientationAngle NOTIFY orientationAngleChanged);
  Q_ENUMS(OrientationDirection);

public:
  Orientation(QObject *parent = 0);
  ~Orientation();

  // Make sure they are synced with metadata.
  typedef enum {
    Unknown = -1,
    Landscape = 0,
    Portrait = 1,
    InvertedLandscape = 2,
    InvertedPortrait = 3
  } OrientationDirection;

  bool isActive() const;
  void setActive(bool active);

  OrientationDirection orientation() const;

  int orientationAngle() const;
  void setOrientationAngle(int angle);

signals:
  void activeChanged();
  void orientationChanged();
  void orientationAngleChanged();

private slots:
  void readingChanged();

private:
  QOrientationSensor *m_sensor;
  OrientationDirection m_direction;
  int m_orientationAngle;
};

#endif /* ORIENTATION_H */

// -*- c++ -*-

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

#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <QObject>

namespace MeeGo {
  class QmOrientation;
  class QmOrientationReading;
};

class Orientation : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(OrientationDirection orientation READ orientation NOTIFY orientationChanged);

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

signals:
  void activeChanged();
  void orientationChanged();

private slots:
  void orientationChanged(const MeeGo::QmOrientationReading& value);

private:
  MeeGo::QmOrientation *m_orientation;
  OrientationDirection m_direction;
};

#endif /* ORIENTATION_H */

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

#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <qmproximity.h>
#include <QObject>


class Proximity : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(bool close READ isClose NOTIFY closeChanged);

public:
  Proximity(QObject *parent = 0);
  ~Proximity();

  bool isActive() const;
  void setActive(bool active);

  bool isClose() const;

signals:
  void activeChanged();
  void closeChanged();

private slots:
  void proximityChanged(const MeeGo::QmProximityReading& reading);

private:
  MeeGo::QmProximity *m_proximity;
  bool m_close;
};

#endif /* PROXIMITY_H */

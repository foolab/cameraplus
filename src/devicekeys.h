// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#ifndef DEVICE_KEYS_H
#define DEVICE_KEYS_H

#include <QObject>
#include <qmkeys.h>

class DeviceKeys : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);

public:
  DeviceKeys(QObject *parent = 0);
  ~DeviceKeys();

  bool isActive() const;
  void setActive(bool active);

signals:
  void activeChanged();
  void volumeUpPressed();
  void volumeUpReleased();
  void volumeDownPressed();
  void volumeDownReleased();

private slots:
  void keyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state);

private:
  MeeGo::QmKeys *m_keys;
};

#endif /* DEVICE_KEYS_H */

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

#ifndef DEVICE_KEYS_H
#define DEVICE_KEYS_H

#include <QObject>

class QTimer;

class DeviceKeys : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(bool repeat READ isRepeating WRITE doRepeat NOTIFY repeatChanged);

public:
  DeviceKeys(QObject *parent = 0);
  ~DeviceKeys();

  bool isActive() const;
  void setActive(bool active);

  bool isRepeating();
  void doRepeat(bool repeat);

signals:
  void activeChanged();
  void volumeUpPressed();
  void volumeUpReleased();
  void volumeDownPressed();
  void volumeDownReleased();
  void repeatChanged();

protected:
  bool eventFilter(QObject *obj, QEvent *event);

private slots:
  void repeatEventsIfNeeded();

private:
  void processKeyEvent(int key, int event);

  bool m_repeating;
  bool m_active;
  QTimer *m_timer;
  int m_event;
  int m_key;
};

#endif /* DEVICE_KEYS_H */

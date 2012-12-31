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

#ifndef BATTERY_INFO_H
#define BATTERY_INFO_H

#include <QObject>

namespace MeeGo {
  class QmBattery;
};

class BatteryInfo : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool charging READ isCharging NOTIFY chargingChanged);
  Q_PROPERTY(bool critical READ isCritical NOTIFY criticalChanged);
  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);

public:
  BatteryInfo(QObject *parent = 0);
  ~BatteryInfo();

  bool isCharging() const;
  bool isCritical() const;

  bool isActive() const;
  void setActive(bool active);

signals:
  void chargingChanged();
  void criticalChanged();
  void activeChanged();

private:
  MeeGo::QmBattery *m_battery;
};

#endif /* BATTERY_INFO_H */

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

#ifndef BATTERY_INFO_H
#define BATTERY_INFO_H

#include <QObject>

class QBatteryInfo;

class BatteryInfo : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(bool good READ isGood NOTIFY isGoodChanged);

public:
  BatteryInfo(QObject *parent = 0);
  ~BatteryInfo();

  bool isActive() const;
  void setActive(bool active);

  bool isGood() const;

signals:
  void activeChanged();
  void isGoodChanged();

private slots:
  void check();

private:
  QBatteryInfo *m_battery;
  bool m_isGood;
};

#endif /* BATTERY_INFO_H */

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

#include "batteryinfo.h"
#include <QBatteryInfo>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

BatteryInfo::BatteryInfo(QObject *parent) :
  QObject(parent),
  m_battery(0),
  m_isGood(false) {

}

BatteryInfo::~BatteryInfo() {
  setActive(false);
}

bool BatteryInfo::isGood() const {
  return m_isGood;
}

bool BatteryInfo::isActive() const {
  return m_battery != 0;
}

void BatteryInfo::setActive(bool active) {
  if (isActive() == active) {
    return;
  }

  if (!active) {
    m_battery->deleteLater();
    m_battery = 0;
  }
  else {
    m_battery = new QBatteryInfo(this);

    QObject::connect(m_battery, SIGNAL(batteryStatusChanged(int, QBatteryInfo::BatteryStatus)),
		     this, SLOT(check()));
    QObject::connect(m_battery, SIGNAL(chargingStateChanged(int, QBatteryInfo::ChargingState)),
		     this, SLOT(check()));
  }

  emit activeChanged();

  if (m_battery) {
    check();
  }
}

void BatteryInfo::check() {
  bool isGood = false;

  if (!m_battery) {
    qmlInfo(this) << "BatteryInfo has to be activated first";
    return;
  }

  if (m_battery->chargingState(0) == QBatteryInfo::Charging) {
    isGood = true;
  } else {
    QBatteryInfo::BatteryStatus state = m_battery->batteryStatus(0);
    if (state == QBatteryInfo::BatteryOk || state == QBatteryInfo::BatteryFull) {
      isGood = true;
    } else {
      isGood = false;
    }
  }

  if (isGood != m_isGood) {
    m_isGood = isGood;
    emit isGoodChanged();
  }
}

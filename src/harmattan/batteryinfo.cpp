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
#include <qmbattery.h>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

BatteryInfo::BatteryInfo(QObject *parent) :
  QObject(parent),
  m_battery(0),
  m_isGood(true) {

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
    m_battery = new MeeGo::QmBattery(this);
    QObject::connect(m_battery, SIGNAL(batteryStateChanged(MeeGo::QmBattery::BatteryState)),
		     this, SLOT(check()));
    QObject::connect(m_battery, SIGNAL(chargingStateChanged(MeeGo::QmBattery::ChargingState)),
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
    isGood = true;
  } else if (m_battery->getChargingState() == MeeGo::QmBattery::StateCharging) {
    isGood = true;
  } else {
    MeeGo::QmBattery::BatteryState state = m_battery->getBatteryState();

    if (state == MeeGo::QmBattery::StateOK || state == MeeGo::QmBattery::StateFull) {
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

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

#include "batteryinfo.h"
#include <qmbattery.h>
#include <QDeclarativeInfo>

BatteryInfo::BatteryInfo(QObject *parent) :
  QObject(parent), m_battery(0) {

}

BatteryInfo::~BatteryInfo() {
  setActive(false);
}

bool BatteryInfo::isCharging() const {
  if (!m_battery) {
    qmlInfo(this) << "BatteryInfo has to be activated first";
    return false;
  }

  if (m_battery->getChargingState() == MeeGo::QmBattery::StateCharging) {
    return true;
  }

  return false;
}

bool BatteryInfo::isCritical() const {
  if (!m_battery) {
    qmlInfo(this) << "BatteryInfo has to be activated first";
    return true;
  }

  MeeGo::QmBattery::BatteryState state = m_battery->getBatteryState();

  if (state == MeeGo::QmBattery::StateOK || state == MeeGo::QmBattery::StateFull) {
    return false;
  }

  return true;
}

bool BatteryInfo::isActive() const {
  return m_battery != 0;
}

void BatteryInfo::setActive(bool active) {
  if (isActive() == active) {
    return;
  }

  m_battery = new MeeGo::QmBattery(this);
  QObject::connect(m_battery, SIGNAL(batteryStateChanged(MeeGo::QmBattery::BatteryState)),
		   this, SIGNAL(chargingChanged()));
  QObject::connect(m_battery, SIGNAL(chargingStateChanged(MeeGo::QmBattery::ChargingState)),
		   this, SIGNAL(chargingChanged()));

  emit activeChanged();
}

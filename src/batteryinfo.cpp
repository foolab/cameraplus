/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2015 Mohammed Sameer <msameer@foolab.org>
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
#include <contextproperty.h>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

#define PERCENTAGE_PROPERTY          "Battery.ChargePercentage"
#define CHARGING_PROPERTY            "Battery.IsCharging"
#define BATTERY_LOW_THRESHOLD        5

BatteryInfo::BatteryInfo(QObject *parent) :
  QObject(parent),
  m_percentage(new ContextProperty(PERCENTAGE_PROPERTY, this)),
  m_charging(new ContextProperty(CHARGING_PROPERTY, this)) {

  m_percentage->waitForSubscription(true);
  m_charging->waitForSubscription(true);

  QObject::connect(m_percentage, SIGNAL(valueChanged()), this, SLOT(check()));
  QObject::connect(m_charging, SIGNAL(valueChanged()), this, SLOT(check()));

  check();
}

BatteryInfo::~BatteryInfo() {

}

bool BatteryInfo::isGood() const {
  return m_isGood;
}

void BatteryInfo::check() {
  bool isGood = false;

  if (m_charging->value().toBool()) {
    isGood = true;
  } else if (m_percentage->value().toInt() > BATTERY_LOW_THRESHOLD) {
    isGood = true;
  }

  if (isGood != m_isGood) {
    m_isGood = isGood;
    emit isGoodChanged();
  }
}

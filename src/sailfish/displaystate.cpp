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

#include "displaystate.h"
#include <contextproperty.h>
#include <QTimer>
#include <QQmlInfo>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>

#define CONTEXT_PROPERTY "Screen.Blanked"
#define DBUS_IFACE       "com.nokia.mce.request"
#define DBUS_PATH        "/com/nokia/mce/request"
#define DBUS_DEST        "com.nokia.mce"
#define BLANKING_PAUSE   "req_display_blanking_pause"
#define BLANKING_RESUME  "req_display_cancel_blanking_pause"

DisplayState::DisplayState(QObject *parent) :
  QObject(parent),
  m_state(new ContextProperty(CONTEXT_PROPERTY, this)),
  m_timer(new QTimer(this)) {

  m_timer->setSingleShot(false);
  m_timer->setInterval(50 * 1000);

  QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
  QObject::connect(m_state, SIGNAL(valueChanged()), this, SIGNAL(isOnChanged()));

  m_state->waitForSubscription(true);
}

DisplayState::~DisplayState() {
  setInhibitDim(false);
}

bool DisplayState::isDimInhibited() const {
  return m_timer->isActive();
}

void DisplayState::setInhibitDim(bool inhibit) {
  if (m_timer->isActive() == inhibit) {
    return;
  }

  if (!inhibit) {
    QDBusMessage msg =
      QDBusMessage::createMethodCall(DBUS_DEST, DBUS_PATH, DBUS_IFACE, BLANKING_RESUME);

    QDBusConnection conn = QDBusConnection::systemBus();
    if (!conn.isConnected()) {
      qmlInfo(this) << "Unable to connect to DBus system bus.";
    } else {
      conn.asyncCall(msg);
    }

    m_timer->stop();
  }
  else {
    timeout();
    m_timer->start();
  }

  emit inhibitDimChanged();
}

void DisplayState::timeout() {
  QDBusMessage msg =
    QDBusMessage::createMethodCall(DBUS_DEST, DBUS_PATH, DBUS_IFACE, BLANKING_PAUSE);

  QDBusConnection conn = QDBusConnection::systemBus();
  if (!conn.isConnected()) {
    qmlInfo(this) << "Unable to connect to DBus system bus.";
  } else {
    conn.asyncCall(msg);
  }
}

bool DisplayState::isOn() {
  return m_state->value().toInt() != 1;
}

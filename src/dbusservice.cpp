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

#include "dbusservice.h"
#include <QDBusInterface>
#include "platformsettings.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif
#include <QDBusPendingCall>

// We cannot use A DBusInterface because it will try to introspect the service upon
// creation. This will lead to services being launched when we create the interfaces.
DbusService::DbusService(QObject *parent) :
  QObject(parent),
  m_settings(0),
  m_enabled(false) {

}

DbusService::~DbusService() {

}

PlatformSettings *DbusService::settings() const {
  return m_settings;
}

void DbusService::setSettings(PlatformSettings *settings) {
  if (m_settings != settings) {
    m_settings = settings;

    init();

    emit settingsChanged();
  }
}

void DbusService::setName(const QString& name) {
  if (m_name != name) {
    m_name = name;

    init();

    emit nameChanged();
  }
}

bool DbusService::isEnabled() const {
  return m_enabled;
}

void DbusService::init() {
  if (m_name.isEmpty() || !m_settings) {
    return;
  }

  PlatformSettings::Service service = m_settings->service(m_name);

  if (service.m_type != "dbus") {
    qmlInfo(this) << "Unknown service type" << service.m_type;
    return;
  }

  if (!service.m_enabled) {
    qmlInfo(this) << "Not initializing a disabled service";
    return;
  }

  m_path = service.m_path;
  m_interface = service.m_interface;
  m_dest = service.m_dest;
  m_method = service.m_method;
  m_enabled = service.m_enabled;
  emit isEnabledChanged();
}

bool DbusService::asyncCall(const QVariant& arg) {
  if (!m_settings) {
    qmlInfo(this) << "Empty settings.";
    return false;
  }

  if (!m_enabled) {
    qmlInfo(this) << "Service is not enabled.";
    return false;
  }

  if (m_dest.isEmpty() || m_path.isEmpty() || m_interface.isEmpty() || m_method.isEmpty()) {
    qmlInfo(this) << "Unable to construct DBus method call.";
    return false;
  }

  QDBusMessage msg = QDBusMessage::createMethodCall(m_dest, m_path, m_interface, m_method);
  msg.setAutoStartService(true);
  if (arg.isValid()) {
    msg.setArguments(QList<QVariant>() << arg);
  }

  QDBusConnection conn = QDBusConnection::sessionBus();
  if (!conn.isConnected()) {
    qmlInfo(this) << "Unable to connect to DBus session bus.";
    return false;
  }

  conn.asyncCall(msg);

  return true;
}

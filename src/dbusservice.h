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

#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#include <QObject>
#include <QVariant>
#include <QHash>

class PlatformSettings;

class DbusService : public QObject {
  Q_OBJECT

  Q_PROPERTY(PlatformSettings* settings READ settings WRITE setSettings NOTIFY settingsChanged);
  Q_PROPERTY(bool enabled READ isEnabled NOTIFY isEnabledChanged);

public:
  DbusService(QObject *parent = 0);
  ~DbusService();

  PlatformSettings *settings() const;
  void setSettings(PlatformSettings *settings);

  void setName(const QString& name);

  bool isEnabled() const;

  bool asyncCall(const QString& method, const QVariant& arg = QVariant());

signals:
  void settingsChanged();
  void nameChanged();
  void isEnabledChanged();

private:
  void init();

  PlatformSettings *m_settings;
  QString m_name;

  QString m_path;
  QString m_interface;
  QString m_dest;

  bool m_enabled;

  QMap<QString, QString> m_methods;
};

#endif /* DBUS_SERVICE_H */

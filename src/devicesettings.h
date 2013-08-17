// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include <QString>
#include <QVariant>
#include <QHash>

class DeviceSettings {
public:
  virtual ~DeviceSettings();

  virtual QString id() const = 0;

  QVariant defaultImageResolution() const;
  QVariant defaultVideoResolution() const;
  QVariant defaultImageAspectRatio() const;
  QVariant defaultVideoAspectRatio() const;
  QVariant defaultImageSceneMode() const;
  QVariant defaultVideoSceneMode() const;
  QVariant defaultImageColorFilter() const;
  QVariant defaultVideoColorFilter() const;
  QVariant defaultImageWhiteBalance() const;
  QVariant defaultVideoWhiteBalance() const;
  QVariant defaultImageEvComp() const;
  QVariant defaultVideoEvComp() const;
  QVariant defaultImageFlashMode() const;
  QVariant defaultImageIso() const;
  QVariant defaultVideoTorchOn() const;

protected:
  DeviceSettings(const QHash<QString, QVariant>& defaultSettings);

private:
  QVariant value(const QString& key) const;

  const QHash<QString, QVariant> m_settings;
};

class PrimaryDeviceSettings : public DeviceSettings {
public:
  PrimaryDeviceSettings();

  QString id() const;

private:
  QHash<QString, QVariant> settings() const;
};

class SecondaryDeviceSettings : public DeviceSettings {
public:
  SecondaryDeviceSettings();

  QString id() const;

private:
  QHash<QString, QVariant> settings() const;
};

#endif /* DEVICE_SETTINGS_H */

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

#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <QObject>

class QDeviceInfo;

class DeviceInfo : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString manufacturer READ manufacturer CONSTANT);
  Q_PROPERTY(QString model READ model CONSTANT);

public:
  DeviceInfo(QObject *parent = 0);
  ~DeviceInfo();

  QString manufacturer();
  QString model();

private:
  QDeviceInfo *m_info;

  QString m_manufacturer;
  QString m_model;
};

#endif /* DEVICE_INFO_H */

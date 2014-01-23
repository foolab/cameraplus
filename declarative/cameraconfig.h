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

#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H

#include <QObject>

class QtCamConfig;

class CameraConfig : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString imageSuffix READ imageSuffix CONSTANT);
  Q_PROPERTY(QString videoSuffix READ videoSuffix CONSTANT);

public:
  CameraConfig(QObject *parent = 0);
  ~CameraConfig();

  QtCamConfig *config() const;

  QString imageSuffix() const;
  QString videoSuffix() const;

private:
  QtCamConfig *m_config;
};

#endif /* CAMERA_CONFIG_H */

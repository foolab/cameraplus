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

#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H

#include <QObject>
#if defined(QT4)
#include <QDeclarativeParserStatus>
#elif defined(QT5)
#include <QQmlParserStatus>
#endif

class QtCamConfig;

#if defined(QT4)
class CameraConfig : public QObject, public QDeclarativeParserStatus {
#elif defined(QT5)
class CameraConfig : public QObject, public QQmlParserStatus {
#endif

  Q_OBJECT

  Q_PROPERTY(QString configPath READ configPath WRITE setConfigPath NOTIFY configPathChanged);

public:
  CameraConfig(QObject *parent = 0);
  ~CameraConfig();

  QString configPath() const;
  void setConfigPath(const QString& configPath);

  QtCamConfig *config() const;

  virtual void classBegin();
  virtual void componentComplete();

signals:
  void configPathChanged();

private:
  QString m_path;
  QtCamConfig *m_config;
};

#endif /* CAMERA_CONFIG_H */

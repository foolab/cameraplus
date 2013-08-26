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

#include "cameraconfig.h"
#include "qtcamconfig.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

// TODO: share that with qtcamera. We now have 2 instances of QtCamConfig

CameraConfig::CameraConfig(QObject *parent) :
  QObject(parent),
  m_config(0) {

}

CameraConfig::~CameraConfig() {
  m_config = 0;
}

QString CameraConfig::configPath() const {
  return m_path;
}

void CameraConfig::setConfigPath(const QString& configPath) {
  if (m_config) {
    qmlInfo(this) << "configPath cannot be changed after initialization";
    return;
  }

  if (m_path != configPath) {
    m_path = configPath;
    emit configPathChanged();
  }
}

QtCamConfig *CameraConfig::config() const {
  return m_config;
}

void CameraConfig::classBegin() {
#if defined(QT4)
  QDeclarativeParserStatus::classBegin();
#elif defined(QT5)
  QQmlParserStatus::classBegin();
#endif

  // Nothing
}

void CameraConfig::componentComplete() {
#if defined(QT4)
  QDeclarativeParserStatus::componentComplete();
#elif defined(QT5)
  QQmlParserStatus::componentComplete();
#endif

  if (m_config) {
    return;
  }

  if (m_path.isEmpty()) {
    m_config = new QtCamConfig(this);
  }
  else {
    m_config = new QtCamConfig(m_path, this);
  }
}

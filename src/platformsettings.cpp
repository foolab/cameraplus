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

#include "platformsettings.h"
#include <QDir>
#include <QSettings>
#include <Quill>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

#ifdef HARMATTAN
#define PATH "/usr/share/cameraplus/config/cameraplus.ini"
#else
#define PATH "/usr/share/harbour-cameraplus/share/cameraplus/config/cameraplus.ini"
#endif

#ifdef HARMATTAN
#define IMAGE_PATH QString("%1%2MyDocs%2DCIM%2").arg(QDir::homePath()).arg(QDir::separator())
#define VIDEO_PATH IMAGE_PATH
#define TEMP_VIDEO QString("%1%2MyDocs%2.cameraplus%2").arg(QDir::homePath()).arg(QDir::separator())
#endif

#ifdef SAILFISH
#define IMAGE_PATH QString("%1%2Pictures%2Camera%2").arg(QDir::homePath()).arg(QDir::separator())
#define VIDEO_PATH QString("%1%2Videos%2Camera%2").arg(QDir::homePath()).arg(QDir::separator())
#define TEMP_VIDEO VIDEO_PATH
#endif

PlatformSettings::PlatformSettings(QObject *parent) :
  QObject(parent),
  m_settings(new QSettings(PATH, QSettings::IniFormat)) {

}

PlatformSettings::~PlatformSettings() {
  delete m_settings;
}

PlatformSettings::Service PlatformSettings::service(const QString& id) {
  PlatformSettings::Service service;
  m_settings->beginGroup(id);

  service.m_enabled = m_settings->value("enabled", false).toBool();
  service.m_type = m_settings->value("type").toString();
  service.m_path = m_settings->value("path").toString();
  service.m_interface = m_settings->value("interface").toString();
  service.m_dest = m_settings->value("dest").toString();
  service.m_method = m_settings->value("method").toString();

  m_settings->endGroup();

  return service;
}

QString PlatformSettings::imageCaptureStartedSound() const {
  return m_settings->value("sounds/imageCaptureStarted").toString();
}

QString PlatformSettings::imageCaptureEndedSound() const {
  return m_settings->value("sounds/imageCaptureEnded").toString();
}

QString PlatformSettings::videoRecordingStartedSound() const {
  return m_settings->value("sounds/videoRecordingStarted").toString();
}

QString PlatformSettings::videoRecordingEndedSound() const {
  return m_settings->value("sounds/videoRecordingEnded").toString();
}

QString PlatformSettings::autoFocusAcquiredSound() const {
  return m_settings->value("sounds/autoFocusAcquired").toString();
}

QString PlatformSettings::autoFocusFailedSound() const {
  return m_settings->value("sounds/autoFocusFailed").toString();
}

QString PlatformSettings::imagePath() {
  if (m_image.isEmpty()) {
    m_image = canonicalPath(IMAGE_PATH);
  }

  return m_image;
}

QString PlatformSettings::videoPath() {
  if (m_video.isEmpty()) {
    m_video = canonicalPath(VIDEO_PATH);
  }

  return m_video;
}

QString PlatformSettings::temporaryVideoPath() {
  if (m_temp.isEmpty()) {
    m_temp = canonicalPath(TEMP_VIDEO);
  }

  return m_temp;
}

QString PlatformSettings::canonicalPath(const QString& path) const {
  if (!QDir::root().mkpath(path)) {
    qmlInfo(this) << "Failed to create path" << path;
    return QString();
  }

  QString newPath = QFileInfo(path).canonicalFilePath();

  if (newPath.isEmpty()) {
    return newPath;
  }

  if (!newPath.endsWith(QDir::separator())) {
    newPath.append(QDir::separator());
  }

  return newPath;
}

int PlatformSettings::maximumVideoDuration() const {
#ifdef SAILFISH
  return -1;
#else
  return 3600;
#endif
}

int PlatformSettings::naturalOrientationIsLandscape() const {
#ifdef SAILFISH
  return false;
#else
  return true;
#endif
}

int PlatformSettings::primarySensorOrientationAngle() const {
#ifdef SAILFISH
  return -1;
#else
  return 0;
#endif
}

int PlatformSettings::secondarySensorOrientationAngle() const {
#ifdef SAILFISH
  return -1;
#else
  return 0;
#endif
}

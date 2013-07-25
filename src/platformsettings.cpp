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

#include "platformsettings.h"
#include <QDir>
#include <QSettings>
#include <Quill>
#include "quillitem.h"

#define PATH "/usr/share/cameraplus/config/cameraplus.ini"

#define PREVIEW_SIZE                             QSize(854, 480)
#define THUMBNAIL_FLAVOR_NAME                    QString("screen")
#define TEMPORARY_FILE_PATH                      "%1%2.config%2quill%2tmp"
#define THUMBNAIL_EXTENSION                      QString("jpeg")
#define THUMBNAIL_CREATION_ENABLED               true
#define DBUS_THUMBNAILING_ENABLED                true
#define BACKGROUND_RENDERING_COLOR               QColor(Qt::black)

PlatformSettings::PlatformSettings(QObject *parent) :
  QObject(parent), m_settings(new QSettings(PATH, QSettings::IniFormat)) {

}

PlatformSettings::~PlatformSettings() {
  delete m_settings;
}

QSize PlatformSettings::previewSize() {
  return m_settings->value("quill/previewSize", PREVIEW_SIZE).toSize();
}

QString PlatformSettings::thumbnailFlavorName() {
  return m_settings->value("quill/thumbnailFlavorName", THUMBNAIL_FLAVOR_NAME).toString();
}

QString PlatformSettings::thumbnailExtension() {
  return m_settings->value("quill/thumbnailExtension", THUMBNAIL_EXTENSION).toString();
}

QColor PlatformSettings::backgroundRenderingColor() {
  return m_settings->value("quill/backgroundRenderingColor",
			   BACKGROUND_RENDERING_COLOR).value<QColor>();
}

bool PlatformSettings::isDBusThumbnailingEnabled() {
  return m_settings->value("quill/dbusThumbnailingEnabled", DBUS_THUMBNAILING_ENABLED).toBool();
}

bool PlatformSettings::isThumbnailCreationEnabled() {
  return m_settings->value("quill/thumbnailCreationEnabled", THUMBNAIL_CREATION_ENABLED).toBool();
}

QString PlatformSettings::temporaryFilePath() {
  QString defaultPath = QString(TEMPORARY_FILE_PATH).arg(QDir::homePath()).arg(QDir::separator());
  return m_settings->value("quill/temporaryFilePath", defaultPath).toString();
}

QSize PlatformSettings::portraitSize(const QSize& size) {
  return size.width() > size.height() ? QSize(size.height(), size.width()) : size;
}

QSize PlatformSettings::landscapeSize(const QSize& size) {
  return size.width() > size.height() ? size : QSize(size.height(), size.width());
}

void PlatformSettings::init() {
  // How we create thumbnails for portrait is really messy.
  // I am sure there is a better way to tell Quill to generate proper
  // portrait thumbnails without having 2 display levels but I don't know how.
  // The issue is we generate screen sized thumbnails for landscape
  // but we generate half screen sized thumbnails for portrait
  Quill::setPreviewLevelCount(2);
  QSize size = previewSize();

  // Landscape:
  Quill::setThumbnailFlavorName(LANDSCAPE_PREVIEW_LEVEL, thumbnailFlavorName());
  Quill::setPreviewSize(LANDSCAPE_PREVIEW_LEVEL, landscapeSize(size));

  // Portrait:
  Quill::setThumbnailFlavorName(PORTRAIT_PREVIEW_LEVEL, thumbnailFlavorName());
  Quill::setPreviewSize(PORTRAIT_PREVIEW_LEVEL, portraitSize(size));

  Quill::setThumbnailExtension(thumbnailExtension());
  Quill::setBackgroundRenderingColor(backgroundRenderingColor());
  Quill::setDBusThumbnailingEnabled(isDBusThumbnailingEnabled());
  Quill::setThumbnailCreationEnabled(isThumbnailCreationEnabled());

  QString tempPath = temporaryFilePath();
  QDir().mkpath(tempPath);
  Quill::setTemporaryFilePath(tempPath);
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

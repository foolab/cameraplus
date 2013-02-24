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
#include <QSize>

#define PATH "/usr/share/cameraplus/config/cameraplus.ini"

#define PREVIEW_SIZE                             QSize(854, 480)
#define THUMBNAIL_FLAVOR_NAME                    QString("screen")
#define TEMPORARY_FILE_PATH                      "%1%2.config%2quill%2tmp"
#define THUMBNAIL_EXTENSION                      QString("jpeg")
#define THUMBNAIL_CREATION_ENABLED               true
#define DBUS_THUMBNAILING_ENABLED                true
#define BACKGROUND_RENDERING_COLOR               QColor(Qt::black)

PlatformSettings::PlatformSettings() :
  QSettings(PATH, QSettings::IniFormat) {

}

PlatformSettings::~PlatformSettings() {

}

QList<QPair<QString, QSize> > PlatformSettings::previewLevels() {
  return QList<QPair<QString, QSize> >() <<
    qMakePair<QString, QSize>(value("quill/thumbnailFlavorName", THUMBNAIL_FLAVOR_NAME).toString(),
			      value("quill/previewSize", PREVIEW_SIZE).toSize());
}

QString PlatformSettings::thumbnailExtension() {
  return value("quill/thumbnailExtension", THUMBNAIL_EXTENSION).toString();
}

QColor PlatformSettings::backgroundRenderingColor() {
  return value("quill/backgroundRenderingColor", BACKGROUND_RENDERING_COLOR).value<QColor>();
}

bool PlatformSettings::isDBusThumbnailingEnabled() {
  return value("quill/dbusThumbnailingEnabled", DBUS_THUMBNAILING_ENABLED).toBool();
}

bool PlatformSettings::isThumbnailCreationEnabled() {
  return value("quill/thumbnailCreationEnabled", THUMBNAIL_CREATION_ENABLED).toBool();
}

QString PlatformSettings::temporaryFilePath() {
  QString defaultPath = QString(TEMPORARY_FILE_PATH).arg(QDir::homePath()).arg(QDir::separator());
  return value("quill/temporaryFilePath", defaultPath).toString();
}

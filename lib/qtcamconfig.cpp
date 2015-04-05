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

#include "qtcamconfig.h"
#include "qtcamresolution.h"
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QMetaEnum>
#include <QFile>
#ifdef QT4
#include <QSystemDeviceInfo>
#else
#include <QDeviceInfo>
#endif
#include "qtcamutils.h"
#include "qtcamconfig_p.h"

#ifndef DATA_DIR
#define DATA_DIR                              "/usr/share/qtcamera/config/"
#endif /* DATA_DIR */

QtCamConfig::QtCamConfig(QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamConfigPrivate(this)) {

#ifdef QT4
  d_ptr->model = QtMobility::QSystemDeviceInfo().productName().toLower();
#else
  d_ptr->model = QDeviceInfo().productName().toLower();
#endif

  d_ptr->init(DATA_DIR);
}

QtCamConfig::~QtCamConfig() {
  delete d_ptr;
}

QString QtCamConfig::lookUp(const QString& path) const {
  QString finalPath = QString("%1/%2/%3").arg(DATA_DIR).arg(d_ptr->model).arg(path);

  if (!QFile::exists(finalPath)) {
    finalPath = QString("%1/%2").arg(DATA_DIR).arg(path);
  }

  return finalPath;
}

QString QtCamConfig::deviceScannerType() const {
  return d_ptr->confValue("devices/scanner").toString();
}

QString QtCamConfig::deviceScannerProperty() const {
  return d_ptr->confValue("devices/property").toString();
}

QString QtCamConfig::videoSource() const {
  return d_ptr->confValue("video-source/element").toString();
}

QString QtCamConfig::viewfinderSink() const {
  return d_ptr->confValue("viewfinder-sink/element").toString();
}

QString QtCamConfig::viewfinderRenderer() const {
  return d_ptr->confValue("viewfinder-sink/renderer").toString();
}

bool QtCamConfig::viewfinderUseFence() const {
  return d_ptr->confValue("viewfinder-sink/use-fence").toBool();
}

QString QtCamConfig::audioSource() const {
  return d_ptr->confValue("audio-source/element").toString();
}

QString QtCamConfig::wrapperVideoSource() const {
  return d_ptr->confValue("wrapper-video-source/element").toString();
}

QString QtCamConfig::wrapperVideoSourceProperty() const {
  return d_ptr->confValue("wrapper-video-source/property").toString();
}

QString QtCamConfig::imageEncodingProfileName() const {
  return d_ptr->confValue("image/profile-name").toString();
}

QString QtCamConfig::imageEncodingProfilePath() const {
  return d_ptr->confValue("image/profile-path").toString();
}

QString QtCamConfig::videoEncodingProfileName() const {
  return d_ptr->confValue("video/profile-name").toString();
}

QString QtCamConfig::videoEncodingProfilePath() const {
  return d_ptr->confValue("video/profile-path").toString();
}

QString QtCamConfig::audioCaptureCaps() const {
  return d_ptr->confValue("audio-capture-caps/caps").toString();
}

QString QtCamConfig::imageSuffix() const {
  return d_ptr->confValue("image/extension").toString();
}

QString QtCamConfig::videoSuffix() const {
  return d_ptr->confValue("video/extension").toString();
}

QStringList QtCamConfig::viewfinderFilters() const {
  return d_ptr->confValue("viewfinder-filters/elements").toStringList();
}

bool QtCamConfig::viewfinderFiltersUseAnalysisBin() const {
  return d_ptr->confValue("viewfinder-filters/use-analysis-bin").toBool();
}

QStringList QtCamConfig::imageFilters() const {
  return d_ptr->confValue("image-filters/elements").toStringList();
}

bool QtCamConfig::imageFiltersUseAnalysisBin() const {
  return d_ptr->confValue("image-filters/use-analysis-bin").toBool();
}

QString QtCamConfig::roiElement() const {
  return d_ptr->confValue("roi/element").toString();
}

QString QtCamConfig::roiMessageName() const {
  return d_ptr->confValue("roi/message").toString();
}

QString QtCamConfig::roiEnableProperty() const {
  return d_ptr->confValue("roi/enable").toString();
}

QString QtCamConfig::roiMessage() const {
  return d_ptr->confValue("roi/message").toString();
}

bool QtCamConfig::isPreviewSupported() const {
  return d_ptr->confValue("General/preview-supported").toBool();
}

QString QtCamConfig::mediaType(const QString& id) const {
  return d_ptr->confValue(QString("media-type/%1").arg(id)).toString();
}

QString QtCamConfig::mediaFourcc(const QString& id) const {
  return d_ptr->confValue(QString("media-type/%1-fourcc").arg(id)).toString();
}

QString QtCamConfig::fastCaptureProperty() const {
  return d_ptr->confValue("fast-capture/property").toString();
}

QString QtCamConfig::resolutionsProvider() const {
  return d_ptr->confValue("resolutions/provider").toString();
}

int QtCamConfig::resolutionsImageFps() const {
  return d_ptr->confValue("resolutions/imageFps").toInt();
}

int QtCamConfig::resolutionsVideoFps() const {
  return d_ptr->confValue("resolutions/videoFps").toInt();
}

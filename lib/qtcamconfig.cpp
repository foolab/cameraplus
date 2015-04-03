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
#include "qtcamimagesettings.h"
#include "qtcamvideosettings.h"
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
  d_ptr(new QtCamConfigPrivate) {

#ifdef QT4
  d_ptr->model = QtMobility::QSystemDeviceInfo().productName().toLower();
#else
  d_ptr->model = QDeviceInfo().productName().toLower();
#endif

  d_ptr->conf = new QSettings(lookUp("qtcamera.ini"), QSettings::IniFormat, this);
  d_ptr->resolutions = new QSettings(lookUp("resolutions.ini"), QSettings::IniFormat, this);
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
  return d_ptr->conf->value("devices/scanner").toString();
}

QString QtCamConfig::deviceScannerProperty() const {
  return d_ptr->conf->value("devices/property").toString();
}

QString QtCamConfig::videoSource() const {
  return d_ptr->element("video-source");
}

QString QtCamConfig::viewfinderSink() const {
  return d_ptr->element("viewfinder-sink");
}

QString QtCamConfig::viewfinderRenderer() const {
  return d_ptr->conf->value("viewfinder-sink/renderer").toString();
}

bool QtCamConfig::viewfinderUseFence() const {
  return d_ptr->conf->value("viewfinder-sink/use-fence").toBool();
}

QString QtCamConfig::audioSource() const {
  return d_ptr->element("audio-source");
}

QString QtCamConfig::wrapperVideoSource() const {
  return d_ptr->element("wrapper-video-source");
}

QString QtCamConfig::wrapperVideoSourceProperty() const {
  return d_ptr->conf->value("wrapper-video-source/property").toString();
}

QtCamImageSettings *QtCamConfig::imageSettings(const QVariant& id) {
  QString generic = "image";
  QString specific = QString("%1-%2").arg(generic).arg(id.toString());

  QString profileName = d_ptr->readWithFallback(generic, specific, "profile-name").toString();
  QString profilePath = d_ptr->readWithFallback(generic, specific, "profile-path").toString();
  QString suffix = d_ptr->readWithFallback(generic, specific, "extension").toString();

  QList<QtCamResolution> resolutions = d_ptr->readResolutions(QtCamResolution::ModeImage,
							      id);

  return new QtCamImageSettings(id.toString(), suffix, profileName, profilePath, resolutions);
}

QtCamVideoSettings *QtCamConfig::videoSettings(const QVariant& id) {
  QString generic = "video";
  QString specific = QString("%1-%2").arg(generic).arg(id.toString());

  QString profileName = d_ptr->readWithFallback(generic, specific, "profile-name").toString();
  QString profilePath = d_ptr->readWithFallback(generic, specific, "profile-path").toString();
  QString suffix = d_ptr->readWithFallback(generic, specific, "extension").toString();

  QList<QtCamResolution> resolutions = d_ptr->readResolutions(QtCamResolution::ModeVideo,
							      id);

  return new QtCamVideoSettings(id.toString(), suffix, profileName, profilePath, resolutions);
}

QString QtCamConfig::imageEncodingProfileName() const {
  return d_ptr->conf->value("image/profile-name").toString();
}

QString QtCamConfig::imageEncodingProfilePath() const {
  return d_ptr->conf->value("image/profile-path").toString();
}

QString QtCamConfig::videoEncodingProfileName() const {
  return d_ptr->conf->value("video/profile-name").toString();
}

QString QtCamConfig::videoEncodingProfilePath() const {
  return d_ptr->conf->value("video/profile-path").toString();
}

QString QtCamConfig::audioCaptureCaps() const {
  return d_ptr->conf->value("audio-capture-caps/caps").toString();
}

QString QtCamConfig::imageSuffix() const {
  return d_ptr->conf->value("image/extension").toString();
}

QString QtCamConfig::videoSuffix() const {
  return d_ptr->conf->value("video/extension").toString();
}

QStringList QtCamConfig::viewfinderFilters() const {
  return d_ptr->conf->value("viewfinder-filters/elements").toStringList();
}

bool QtCamConfig::viewfinderFiltersUseAnalysisBin() const {
  return d_ptr->conf->value("viewfinder-filters/use-analysis-bin").toBool();
}

QStringList QtCamConfig::imageFilters() const {
  return d_ptr->conf->value("image-filters/elements").toStringList();
}

bool QtCamConfig::imageFiltersUseAnalysisBin() const {
  return d_ptr->conf->value("image-filters/use-analysis-bin").toBool();
}

QString QtCamConfig::roiElement() const {
  return d_ptr->conf->value("roi/element").toString();
}

QString QtCamConfig::roiMessageName() const {
  return d_ptr->conf->value("roi/message").toString();
}

QString QtCamConfig::roiEnableProperty() const {
  return d_ptr->conf->value("roi/enable").toString();
}

QString QtCamConfig::roiMessage() const {
  return d_ptr->conf->value("roi/message").toString();
}

bool QtCamConfig::isPreviewSupported() const {
  return d_ptr->conf->value("general/preview-supported").toBool();
}

QString QtCamConfig::mediaType(const QString& id) const {
  return d_ptr->conf->value(QString("media-type/%1").arg(id)).toString();
}

QString QtCamConfig::mediaFourcc(const QString& id) const {
  return d_ptr->conf->value(QString("media-type/%1-fourcc").arg(id)).toString();
}

QString QtCamConfig::fastCaptureProperty() const {
  return d_ptr->conf->value("fast-capture/property").toString();
}

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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
#include <QSettings>
#include <QStringList>
#include <QDebug>

#define CONFIGURATION_FILE                    "/etc/qtcamera/qtcamera.ini"

class QtCamConfigPrivate {
public:
  QString element(const QString& name) const {
    return conf->value(QString("%1/element").arg(name)).toString();
  }

  QSize readResolution(const QString key) {
    QList<QString> parts = conf->value(key).toString().trimmed().split("x");
    return QSize(parts[0].toInt(), parts[1].toInt());
  }

  QVariant readWithFallback(const QString& generic, const QString& specific, const QString& key) {
    QString genericKey = QString("%1/%2").arg(generic).arg(key);
    QString specificKey = QString("%1/%2").arg(specific).arg(key);

    QVariant var = conf->value(genericKey);

    return conf->value(specificKey, var);
  }

  QSettings *conf;

  QList<QtCamImageSettings> imageSettings;
  QList<QtCamVideoSettings> videoSettings;
};

QtCamConfig::QtCamConfig(QObject *parent) :
  QObject(parent), d_ptr(new QtCamConfigPrivate) {

  d_ptr->conf = new QSettings(CONFIGURATION_FILE, QSettings::IniFormat, this);
}

QtCamConfig::QtCamConfig(const QString& configPath, QObject *parent) :
  QObject(parent), d_ptr(new QtCamConfigPrivate) {

  d_ptr->conf = new QSettings(configPath, QSettings::IniFormat, this);
}

QtCamConfig::~QtCamConfig() {
  delete d_ptr;
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
  QStringList presets = d_ptr->readWithFallback(generic, specific, "presets").toStringList();

  QList<QtCamImageResolution> resolutions;

  foreach (const QString& preset, presets) {
    d_ptr->conf->beginGroup(preset);

    QString id = preset;
    QString name = d_ptr->conf->value("name").toString();
    QSize capture = d_ptr->readResolution("capture");
    QSize preview = d_ptr->readResolution("preview");
    QSize viewfinder = d_ptr->readResolution("viewfinder");
    int fps = d_ptr->conf->value("fps").toInt();
    int nightFps = d_ptr->conf->value("night").toInt();
    int megaPixels = d_ptr->conf->value("megapixels").toInt();
    QString aspectRatio = d_ptr->conf->value("aspectratio").toString();

    d_ptr->conf->endGroup();

    resolutions << QtCamImageResolution(id, name, capture, preview, viewfinder,
					fps, nightFps, megaPixels, aspectRatio);
  }

  return new QtCamImageSettings(id.toString(), suffix, profileName, profilePath, resolutions);
}

QtCamVideoSettings *QtCamConfig::videoSettings(const QVariant& id) {
  QString generic = "video";
  QString specific = QString("%1-%2").arg(generic).arg(id.toString());

  QString profileName = d_ptr->readWithFallback(generic, specific, "profile-name").toString();
  QString profilePath = d_ptr->readWithFallback(generic, specific, "profile-path").toString();
  QString suffix = d_ptr->readWithFallback(generic, specific, "extension").toString();
  QStringList presets = d_ptr->readWithFallback(generic, specific, "presets").toStringList();

  QList<QtCamVideoResolution> resolutions;

  foreach (const QString& preset, presets) {
    d_ptr->conf->beginGroup(preset);

    QString id = preset;
    QString name = d_ptr->conf->value("name").toString();
    QSize capture = d_ptr->readResolution("capture");
    QSize preview = d_ptr->readResolution("preview");
    int fps = d_ptr->conf->value("fps").toInt();
    int nightFps = d_ptr->conf->value("night").toInt();
    QString aspectRatio = d_ptr->conf->value("aspectratio").toString();
    QString resolution = d_ptr->conf->value("resolution").toString();

    d_ptr->conf->endGroup();

    resolutions << QtCamVideoResolution(id, name, capture, preview,
					fps, nightFps, aspectRatio, resolution);
  }

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

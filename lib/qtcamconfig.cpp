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
#include <cmath>

#ifndef DATA_DIR
#define DATA_DIR                              "/usr/share/qtcamera/config/"
#endif /* DATA_DIR */

#define CONFIGURATION_FILE                    QString("%1/qtcamera.ini")
#define RESOLUTIONS_FILE                      QString("%1/%2resolutions.ini")

class QtCamConfigPrivate {
public:
  QString element(const QString& name) const {
    return conf->value(QString("%1/element").arg(name)).toString();
  }

  QSize readResolution(const QString key) {
    QList<QString> parts = resolutions->value(key).toString().trimmed().split("x");
    return QSize(parts[0].toInt(), parts[1].toInt());
  }

  QVariant readWithFallback(const QString& generic, const QString& specific, const QString& key,
			    QSettings *settings = 0) {
    if (!settings) {
      settings = conf;
    }

    QString genericKey = QString("%1/%2").arg(generic).arg(key);
    QString specificKey = QString("%1/%2").arg(specific).arg(key);

    QVariant var = settings->value(genericKey);

    return settings->value(specificKey, var);
  }

  QSettings *conf;
  QSettings *resolutions;

  QList<QtCamResolution> readResolutions(const QtCamResolution::Mode& targetMode,
					 const QVariant& targetDevice) {
    QMap<int, QtCamResolution> res;

    foreach (const QString& id, resolutions->childGroups()) {
      resolutions->beginGroup(id);

      QString name = resolutions->value("name").toString();
      QSize capture = readResolution("capture");
      QSize preview = readResolution("preview");
      QSize viewfinder = readResolution("viewfinder");
      int fps = resolutions->value("fps").toInt();
      int nightFps = resolutions->value("night").toInt();
      int zslFps = resolutions->value("zsl").toInt();
      float megaPixels = capture.width() * capture.height();
      megaPixels = floor((megaPixels * 10) / (1000.0 * 1000.0)) / 10;

      QString aspectRatio = calculateAspectRatio(capture);
      QString commonName = resolutions->value("resolution").toString();
      QVariant device = resolutions->value("device");

      QtCamResolution::Mode mode;
      QString m = resolutions->value("mode").toString();

      if (m == "image") {
	mode = QtCamResolution::ModeImage;
      }
      else if (m == "video") {
	mode = QtCamResolution::ModeVideo;
      }

      int order = resolutions->value("order", 0).toInt();

      resolutions->endGroup();

      if (targetMode != mode || targetDevice != device) {
	continue;
      }

      QtCamResolution r(id, name, aspectRatio, capture, preview, viewfinder, fps, nightFps,
			zslFps, megaPixels, commonName, mode, device);
      if (r.isValid()) {
	res.insertMulti(order, r);
      }
    }

    return res.values();
  }

  QString calculateAspectRatio(const QSize& size) {
    if (m_ratios.isEmpty()) {
      m_ratios[1.3] = "4:3";
      m_ratios[1.5] = "3:2";
      m_ratios[1.6] = "16:10";
      m_ratios[1.7] = "16:9";
    }

    qDebug() << size;

    float r = (size.width() * 1.0) / size.height();
    r = floor(r * 10) / 10.0;

    for (QMap<float, QString>::const_iterator iter = m_ratios.constBegin();
	 iter != m_ratios.constEnd(); iter++) {
      if (qFuzzyCompare (r, iter.key())) {
	return iter.value();
      }
    }

    qWarning() << "calculateAspectRatio: unknown aspect ratio for size" << size;

    return QString("?:?");
  }

  QMap<float, QString> m_ratios;
};

QtCamConfig::QtCamConfig(QObject *parent) :
  QObject(parent), d_ptr(new QtCamConfigPrivate) {

#ifdef QT4
  QString dev = QtMobility::QSystemDeviceInfo().model();
#else
  QString dev = QDeviceInfo().model();
#endif

  d_ptr->conf = new QSettings(CONFIGURATION_FILE.arg(dir()), QSettings::IniFormat, this);

  QString resolutions = RESOLUTIONS_FILE.arg(dir()).arg(dev.toLower() + "_");
  if (!QFile(resolutions).exists()) {
    resolutions = RESOLUTIONS_FILE.arg(dir()).arg(QString());
  }

  d_ptr->resolutions = new QSettings(resolutions, QSettings::IniFormat, this);
}

QtCamConfig::~QtCamConfig() {
  delete d_ptr;
}

QString QtCamConfig::dir() const {
  return QString::fromLocal8Bit(DATA_DIR);
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

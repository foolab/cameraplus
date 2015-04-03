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

#ifndef QT_CAM_CONFIG_P_H
#define QT_CAM_CONFIG_P_H

#include <QSettings>
#include "qtcamresolution.h"
#include "qtcamutils.h"

class QtCamConfigPrivate {
public:
  QString element(const QString& name) const {
    return conf->value(QString("%1/element").arg(name)).toString();
  }

  QSize readResolution(const QString key) {
    QList<QString> parts = resolutions->value(key).toString().trimmed().split("x");
    return QSize(parts[0].toInt(), parts[1].toInt());
  }

  QVariant readWithFallback(const QString& generic, const QString& specific, const QString& key) {

    QString genericKey = QString("%1/%2").arg(generic).arg(key);
    QString specificKey = QString("%1/%2").arg(specific).arg(key);

    QVariant var = conf->value(genericKey);

    return conf->value(specificKey, var);
  }

  QList<QtCamResolution> readResolutions(const QtCamResolution::Mode& targetMode,
					 const QVariant& targetDevice) {
    QMap<float, QtCamResolution> res;

    foreach (const QString& id, resolutions->childGroups()) {
      resolutions->beginGroup(id);

      QSize capture = readResolution("capture");
      QSize preview = readResolution("preview");
      QSize viewfinder = readResolution("viewfinder");
      int fps = resolutions->value("fps").toInt();
      int nightFps = resolutions->value("night").toInt();
      int zslFps = resolutions->value("zsl").toInt();
      float megaPixels = QtCamUtils::megapixelsForResolution(capture);
      QString aspectRatio = QtCamUtils::aspectRatioForResolution(capture);
      QString commonName = QtCamUtils::commonNameForResolution(capture);
      QVariant device = resolutions->value("device");

      QtCamResolution::Mode mode;
      QString m = resolutions->value("mode").toString();

      if (m == "image") {
	mode = QtCamResolution::ModeImage;
      }
      else if (m == "video") {
	mode = QtCamResolution::ModeVideo;
      }

      resolutions->endGroup();

      if (targetMode != mode || targetDevice != device) {
	continue;
      }

      QtCamResolution r(id, aspectRatio, capture, preview, viewfinder, fps, nightFps,
			zslFps, megaPixels, commonName, mode, device);
      if (r.isValid()) {
	res.insertMulti(-1 * megaPixels, r); // a trick to sort in reverse!
      }
    }

    return res.values();
  }

  QSettings *conf;
  QSettings *resolutions;
  QString model;
};

#endif /* QT_CAM_CONFIG_P_H */

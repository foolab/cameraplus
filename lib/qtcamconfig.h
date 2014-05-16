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

#ifndef QT_CAM_CONFIG_H
#define QT_CAM_CONFIG_H

#include <QObject>
#include <QStringList>

class QtCamImageSettings;
class QtCamVideoSettings;

#define SCANNER_TYPE_V4L2                     "v4l2"
#define SCANNER_TYPE_ENUM                     "enum"

#define RENDERER_TYPE_MEEGO                   "meego"
#define RENDERER_TYPE_NEMO                    "nemo"

#ifndef DATA_DIR
#define DATA_DIR                              "/usr/share/qtcamera/config/"
#endif /* DATA_DIR */

class QtCamConfigPrivate;

class QtCamConfig : public QObject {
  Q_OBJECT

public:
  QtCamConfig(QObject *parent = 0);

  virtual ~QtCamConfig();

  QString deviceScannerType() const;
  QString deviceScannerProperty() const;

  QString videoSource() const;

  QString viewfinderSink() const;
  QString viewfinderRenderer() const;
  bool viewfinderUseFence() const;

  QString audioSource() const;
  QString wrapperVideoSource() const;
  QString wrapperVideoSourceProperty() const;

  QtCamImageSettings *imageSettings(const QVariant& id);
  QtCamVideoSettings *videoSettings(const QVariant& id);

  QString imageEncodingProfileName() const;
  QString imageEncodingProfilePath() const;

  QString videoEncodingProfileName() const;
  QString videoEncodingProfilePath() const;

  QString imageSuffix() const;
  QString videoSuffix() const;

  QString audioCaptureCaps() const;

  QStringList viewfinderFilters() const;
  bool viewfinderFiltersUseAnalysisBin() const;

  QStringList imageFilters() const;
  bool imageFiltersUseAnalysisBin() const;

  QString roiElement() const;
  QString roiMessageName() const;
  QString roiEnableProperty() const;
  QString roiMessage() const;

  bool isPreviewSupported() const;

  QString mediaType(const QString& id) const;

private:
  QtCamConfigPrivate *d_ptr;
};

#endif /* QT_CAM_CONFIG_H */

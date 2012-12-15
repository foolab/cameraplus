// -*- c++ -*-

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

#ifndef QT_CAM_CONFIG_H
#define QT_CAM_CONFIG_H

#include <QObject>
#include "qtcamimagesettings.h"
#include "qtcamvideosettings.h"

#define SCANNER_TYPE_V4L2                     "v4l2"
#define SCANNER_TYPE_ENUM                     "enum"

// TODO: kill those
//#define RENDERER_TYPE_GL_SINK                 "glsink"
//#define RENDERER_TYPE_X_OVERLAY               "xoverlay"
#define RENDERER_TYPE_MEEGO                   "meego"
#define RENDERER_TYPE_GENERIC                 "generic"
class QtCamConfigPrivate;

class QtCamConfig : public QObject {
  Q_OBJECT

public:
  QtCamConfig(QObject *parent = 0);
  QtCamConfig(const QString& configPath, QObject *parent = 0);

  virtual ~QtCamConfig();

  QString deviceScannerType() const;
  QString deviceScannerProperty() const;

  QString videoSource() const;
  QString viewfinderSink() const;
  QString viewfinderRenderer() const;
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

  QString imageCaptureStartedSound() const;
  QString imageCaptureEndedSound() const;
  QString videoRecordingStartedSound() const;
  QString videoRecordingEndedSound() const;
  QString autoFocusAcquiredSound() const;

private:
  QtCamConfigPrivate *d_ptr;
};

#endif /* QT_CAM_CONFIG_H */

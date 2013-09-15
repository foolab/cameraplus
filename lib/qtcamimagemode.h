// -*- c++ -*-

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

#ifndef QT_CAM_IMAGE_MODE_H
#define QT_CAM_IMAGE_MODE_H

#include "qtcammode.h"
#include <gst/pbutils/encoding-profile.h>

class QtCamDevicePrivate;
class QtCamImageModePrivate;
class QtCamImageResolution;
class QtCamImageSettings;

class QtCamImageMode : public QtCamMode {
  Q_OBJECT

public:
  QtCamImageMode(QtCamDevicePrivate *dev, QObject *parent = 0);
  ~QtCamImageMode();

  virtual bool canCapture();
  virtual void applySettings();

  bool capture(const QString& fileName);

  bool setResolution(const QtCamImageResolution& resolution);

  QtCamImageResolution currentResolution();

  void setProfile(GstEncodingProfile *profile);

  QtCamImageSettings *settings() const;

  void enablePreview();

signals:
  void captureStarted();
  void captureEnded();

protected:
  virtual void start();
  virtual void stop();

private:
  QtCamImageModePrivate *d;
};

#endif /* QT_CAM_IMAGE_MODE_H */

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

#ifndef QT_CAM_VIDEO_MODE_H
#define QT_CAM_VIDEO_MODE_H

#include "qtcammode.h"
#include <gst/pbutils/encoding-profile.h>

class QtCamDevicePrivate;
class QtCamVideoModePrivate;
class QtCamVideoResolution;
class QtCamVideoSettings;

class QtCamVideoMode : public QtCamMode {
  Q_OBJECT

  Q_PROPERTY(bool recording READ isRecording NOTIFY recordingStateChanged);

public:
  QtCamVideoMode(QtCamDevicePrivate *dev, QObject *parent = 0);
  ~QtCamVideoMode();

  virtual bool canCapture();
  virtual void applySettings();

  bool isRecording();

  bool startRecording(const QString& fileName, const QString& tmpFileName = QString());

  bool setResolution(const QtCamVideoResolution& resolution);

  QtCamVideoResolution currentResolution();

  void setProfile(GstEncodingProfile *profile);

  QtCamVideoSettings *settings();

public slots:
  void stopRecording(bool sync);

signals:
  void recordingStateChanged();

protected:
  virtual void start();
  virtual void stop();

private:
  QtCamVideoModePrivate *d;
  Q_PRIVATE_SLOT(d, void _d_idleStateChanged(bool));
};

#endif /* QT_CAM_VIDEO_MODE_H */

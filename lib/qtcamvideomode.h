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

#ifndef QT_CAM_VIDEO_MODE_H
#define QT_CAM_VIDEO_MODE_H

#include "qtcammode.h"

class QtCamDevicePrivate;
class QtCamVideoModePrivate;
class QtCamResolution;
class QtCamVideoSettings;

class QtCamVideoMode : public QtCamMode {
  Q_OBJECT

public:
  QtCamVideoMode(QtCamDevicePrivate *dev, QObject *parent = 0);
  ~QtCamVideoMode();

  virtual bool canCapture();
  virtual void applySettings();

  bool isRecording();
  bool isPaused();

  bool startRecording(const QString& fileName, const QString& tmpFileName = QString());

  bool setResolution(const QtCamResolution& resolution);

  QtCamResolution currentResolution();

  QtCamVideoSettings *settings();

  void enablePreview();

public slots:
  void stopRecording(bool sync);
  void pauseRecording(bool pause);

signals:
  void recordingStateChanged();
  void pauseStateChanged();

protected:
  virtual void start();
  virtual void stop();

private:
  QtCamVideoModePrivate *d;
};

#endif /* QT_CAM_VIDEO_MODE_H */

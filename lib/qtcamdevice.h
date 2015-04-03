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

#ifndef QT_CAM_DEVICE_H
#define QT_CAM_DEVICE_H

#include <QObject>
#include <QVariant>

class QtCamDevicePrivate;
class QtCamConfig;
class QtCamViewfinder;
class QtCamVideoMode;
class QtCamImageMode;
class QtCamMode;
class QtCamGstMessageListener;
class QtCamMetaData;
class QtCamCapability;
class QtCamNotifications;
class QtCamViewfinderBufferListener;
class QtCamImageSettings;
class QtCamVideoSettings;

class QtCamDevice : public QObject {
  Q_OBJECT

public:
  QtCamDevice(QtCamConfig *config, const QString& name, const QVariant& id, QObject *parent = 0);
  ~QtCamDevice();

  bool setViewfinder(QtCamViewfinder *viewfinder);
  QtCamViewfinder *viewfinder() const;

  bool start();
  bool stop(bool force);
  bool isRunning();
  bool isIdle();

  QtCamImageMode *imageMode() const;
  QtCamVideoMode *videoMode() const;

  QtCamMode *activeMode() const;

  QString name() const;
  QVariant id() const;

  QtCamConfig *config() const;
  QtCamGstMessageListener *listener() const;
  QtCamViewfinderBufferListener *bufferListener() const;

  QtCamNotifications *notifications() const;

  int sensorOrientationAngle();

  QList<QSize> queryImageResolutions();
  QList<QSize> queryVideoResolutions();
  QList<QSize> queryViewfinderResolutions();

  QtCamImageSettings *imageSettings();
  QtCamVideoSettings *videoSettings();

signals:
  void error(const QString& message, int code, const QString& debug);
  void started();
  void stopping();
  void stopped();
  void idleStateChanged(bool isIdle);
  void modeChanged();
  void runningStateChanged(bool running);
  void sensorOrientationAngleChanged();

private:
  Q_PRIVATE_SLOT(d_ptr, void _d_error(const QString&, int, const QString&))
  Q_PRIVATE_SLOT(d_ptr, void _d_started())
  Q_PRIVATE_SLOT(d_ptr, void _d_stopped())
  Q_PRIVATE_SLOT(d_ptr, void _d_stopping())

  friend class QtCamMetaDataPrivate;
  friend class QtCamCapability;
  friend class QtCamVideoTorch;
  friend class QtCamVideoMute;
  friend class QtCamAutoFocusPrivate;
  friend class QtCamRoiPrivate;
  friend class QtCamNotifications;

  QtCamDevicePrivate *d_ptr;
};

#endif /* QT_CAM_DEVICE_H */
